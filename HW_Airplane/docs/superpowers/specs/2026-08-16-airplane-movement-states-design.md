# 비행기 이동 상태 머신 설계

## 개요

`UAirplaneMovementComponent`는 이미 상태 패턴(State Pattern)으로 뼈대가 잡혀 있다
(`IIAirplaneMovement` 인터페이스, `ChangeState`, 상태 인스턴스 캐시). 이 설계는 그 뼈대 위에
지상 주행부터 이착륙까지 이어지는 4개 상태와 상태 간 전환 조건, 그리고 각 상태의 구체적인
이동 로직을 정의한다.

## 목표

- 활주로 주행 → 이륙 → 비행 → 착륙 준비 → 활주로 복귀로 이어지는 자연스러운 흐름 구현
- 상태 전환은 대부분 자동(속도/고도 조건), 착륙 진입만 플레이어 입력(Landing 키) + 속도 조건으로 트리거
- 각 상태의 튜닝 파라미터는 `UPROPERTY(EditAnywhere)`로 노출하여 코드 재컴파일 없이 조정 가능

## 상태 목록

1. **Ground** (활주로 주행) — 기존 `UGroundAirplaneMovement`, 로직 구현 필요
2. **TakingOff** (이륙중) — 신규 `UTakingOffAirplaneMovement`
3. **Flying** (일반 비행) — 신규 `UFlyingAirplaneMovement`
4. **LandingPrep** (착륙준비) — 신규 `ULandingPrepAirplaneMovement`

```
Ground --[속도 ≥ TakeoffSpeedThreshold]--> TakingOff
TakingOff --[상승고도 ≥ TakeoffCompleteAltitude]--> Flying
Flying --[Landing 키 입력 AND 속도 ≤ LandingSpeedThreshold]--> LandingPrep
       --[Landing 키 입력 AND 속도 > LandingSpeedThreshold]--> (무시, 상태 유지)
LandingPrep --[지면 근접 판정]--> Ground
```

## 아키텍처 변경

### UAirplaneMovementComponent

현재 `SetPitchInput`/`SetRollInput`/`SetYawInput`은 로그만 남기고 값을 저장하지 않는다.
`SetThrottleInput`만 `ThrottleInput` 멤버에 저장한다. 이를 다음과 같이 통일한다.

- `PitchInput`, `RollInput`, `YawInput`, `ThrottleInput` 4개 float 멤버(`[-1, 1]`)를 저장하고,
  상태 클래스에서 읽을 수 있도록 public getter를 추가한다.
- `CurrentSpeed` (float) 를 컴포넌트에 추가한다. 상태 전환 시에도 값이 끊기지 않아야 하는
  런타임 값(현재 속도)은 상태 객체가 아니라 컴포넌트가 소유한다. 상태별 튜닝 파라미터
  (가속도, 임계 속도, 보간 속도 등)는 각 상태 클래스가 `UPROPERTY`로 소유한다.
- `GetOrCreateState<T>()`는 현재 `private`이라 상태 클래스에서 호출할 수 없다. 상태가 스스로
  다음 상태로 전환을 요청할 수 있도록 public 템플릿 `TransitionToState<T>()`를 추가한다
  (내부적으로 `ChangeState(GetOrCreateState<T>())` 호출).
- `ToggleLandingMode()`는 더 이상 로그만 찍지 않고, 현재 상태의
  `IIAirplaneMovement::RequestLanding(this)`를 호출한다. 기본 구현은 아무것도 하지 않으며
  (Ground/TakingOff/LandingPrep 상태에서는 착륙 요청이 의미 없음), `UFlyingAirplaneMovement`만
  이를 오버라이드해 속도 조건을 검사한다.

### IIAirplaneMovement 인터페이스

기존 `Enter`/`Exit`/`TickMovement`(순수 가상)에 다음을 추가한다.

- `virtual void RequestLanding(UAirplaneMovementComponent* MovementComponent) {}` — 기본은 빈 구현
  (순수 가상 아님). `UFlyingAirplaneMovement`에서만 오버라이드.

## 상태별 상세 로직

### 1. Ground — `UGroundAirplaneMovement`

**튜닝 파라미터**: `MaxGroundSpeed`, `GroundAccelInterpSpeed`, `SteeringTurnRate`(속도 0 기준
최대 회전율), `TakeoffSpeedThreshold`

**TickMovement**:
- `CurrentSpeed`를 `Throttle * MaxGroundSpeed`를 목표값으로 `FMath::FInterpTo`로 가속/감속
- Yaw 입력으로 Z축 회전, 회전율은 속도에 비례해 스케일(저속에서 급회전, 고속에서 완만한 회전)
- Pitch/Roll 입력 무시
- 전진 방향(`GetForwardVector() * CurrentSpeed * DeltaTime`)으로 `SafeMoveUpdatedComponent` 이동
- `CurrentSpeed >= TakeoffSpeedThreshold`이면 즉시 `MovementComponent->TransitionToState<UTakingOffAirplaneMovement>()`

### 2. TakingOff — `UTakingOffAirplaneMovement` (신규)

**튜닝 파라미터**: `ClimbPitchTarget`(목표 상승각, 예 15도), `PitchInterpSpeed`,
`TakeoffCompleteAltitude`(이륙 완료로 볼 상승 고도)

**Enter**: 진입 시점의 고도(Z)를 `StartAltitude`에 저장

**TickMovement**:
- `CurrentSpeed`는 Ground와 동일한 방식으로 Throttle 기반 보간 유지
- 현재 Pitch를 `ClimbPitchTarget`까지 `FMath::RInterpTo`로 서서히 들어올림
- Roll/Yaw 입력은 약하게(계수 축소) 반영 가능
- 회전된 전진 방향(기수 방향)으로 이동 → Pitch가 올라가는 만큼 자연스럽게 고도 상승
- `현재 Z - StartAltitude >= TakeoffCompleteAltitude`이면
  `MovementComponent->TransitionToState<UFlyingAirplaneMovement>()`

### 3. Flying — `UFlyingAirplaneMovement` (신규)

**튜닝 파라미터**: `MaxAirSpeed`, `StallSpeed`, `ThrustAccelInterpSpeed`, `RotationInterpSpeed`,
`PitchRate`/`RollRate`/`YawRate`(도/초), `LandingSpeedThreshold`

**TickMovement**:
- `CurrentSpeed`를 `Throttle * MaxAirSpeed` 목표로 `FInterpTo`
- Pitch/Roll/Yaw 입력 × 각 Rate × DeltaTime으로 목표 회전 델타를 구하고 `RInterpTo`로 부드럽게 적용
- 기수 방향(`GetForwardVector()`)이 곧 진행 방향
- `CurrentSpeed < StallSpeed`이면 진행 방향을 월드 아래 방향과 `Lerp`(비율은 실속 정도에 비례)해
  중력에 의해 가라앉는 느낌을 근사
- `SafeMoveUpdatedComponent`로 이동

**RequestLanding** (오버라이드):
- `CurrentSpeed <= LandingSpeedThreshold`이면 `MovementComponent->TransitionToState<ULandingPrepAirplaneMovement>()`
- 조건 불만족 시 아무 동작 없음 (요청은 버려짐, 이후 속도가 낮아진 뒤 다시 눌러야 함)

### 4. LandingPrep — `ULandingPrepAirplaneMovement` (신규)

**튜닝 파라미터**: `MaxLandingSpeed`(하향 조정된 속도 상한), `DescentPitchTarget`(완만한 하강각),
`PitchInterpSpeed`, `GroundProximityThreshold`(지면으로 간주할 거리)

**TickMovement**:
- Flying과 동일한 조종 로직을 재사용하되 목표 속도 상한을 `MaxLandingSpeed`로 낮춤
- Pitch 목표값을 완만한 하강각(`DescentPitchTarget`)으로 유도해 서서히 고도를 낮춤
- 매 틱 현재 위치에서 월드 아래 방향으로 라인트레이스(`LineTraceSingleByChannel`, ECC_Visibility)를
  쏘아 지면까지의 거리를 측정 (부딪히는 지형이 없으면 거리 계산 생략, 상태 전환 없이 하강만 지속)
- 거리 ≤ `GroundProximityThreshold`이면 `MovementComponent->TransitionToState<UGroundAirplaneMovement>()`
  호출 전, 고도/Pitch/Roll을 지면 기준으로 스냅(0으로 정렬)

## 테스트 / 검증 계획

- 자동화 단위 테스트보다는 PIE(Play In Editor) 수동 검증이 중심 (물리 감각·이동 흐름 확인)
- 각 상태 전환 조건을 로그(`UE_LOG`)로 출력해 PIE에서 상태 변화 시점을 확인
- 확인 항목:
  - Ground에서 Throttle을 올려 TakeoffSpeedThreshold를 넘기면 TakingOff로 전환되는지
  - TakingOff 중 고도가 점진적으로 오르고, TakeoffCompleteAltitude 도달 시 Flying으로 전환되는지
  - Flying에서 속도가 높을 때 Landing 키를 눌러도 무시되는지, 감속 후 눌렀을 때 LandingPrep으로 전환되는지
  - LandingPrep에서 서서히 하강해 지면 근접 시 Ground로 복귀하고 착지 자세가 자연스러운지
  - Flying에서 속도가 StallSpeed 밑으로 떨어지면 기체가 가라앉는지

## 범위 밖

- 실제 양력 방정식, 받음각(AoA), 실속 회복 메커니즘 등 정밀 비행 물리는 다루지 않는다
- 지형이 평탄하지 않은 경우의 활주로/착륙 지점 탐색(런웨이 정렬 등)은 다루지 않는다
- 손상/충돌 판정, 연료, 랜딩기어 애니메이션 등 부가 시스템은 이번 설계 범위 밖
