# Unreal Engine 게임 프로젝트 : SimpleShooter

개발 기간 : 2024.12 ~ 2025.01

장르 : 3인칭 슈터

기술 스택: C, C++, STL, Unreal Engine

---

## 시연영상
TODO

---

## 목적 및 설명
언리얼엔진의 스켈레탈 애니메이션, 향상된 입력, UI, AI 비헤이비어트리 등의 학습을 위한 프로젝트입니다.
무료 애셋을 이용해 진행했습니다.

---

## 주요 구현

## 1. Behavior Tree
  - 셀렉터, 시퀀서, 테스크 노드 등의 다양한 노드를 이용해 플레이어를 추적하고, 공격하고, 순찰도는 AI 개발
  - C++로 커스텀 BT_Service 데코레이션, 커스텀 BT_Task 노드를 구현

## 2. Animation Blueprint
  - 애니메이션 블루프린트를 이용해 FSM구축, Character 상태에 따른 애니메이션 전환과 블랜딩을 구현

## 3. GameMode
  - C++로 커스텀 게임모드를 구현, 이벤트 델리게이트를 통해 폰의 상태와 게임 승리조건을 연동시킴

## 4. Character Controller
  - **플레이어 컨트롤러** : 향상된 입력을 이용해 키보드, 게임패드 입력과 내부 함수를 연동
  - **AI 컨트롤러** : 비헤이비어 트리를 통해 AI 제어

## 5. 무기
  - Actor 파생 객체 Gun을 구현, 캐릭터의 스켈레탈 메쉬에서 무기용 본을 찾아 위치를 연동시킴
  - 파생된 블루프린트 객체를 통해 여러 종류의 무기를 구현하고 쉽게 캐릭터에게 장착시킬 수 있음
