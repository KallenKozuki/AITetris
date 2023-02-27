// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKAITetris.generated.h"

UCLASS()
class AITETRIS_API AKKAITetris : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKKAITetris();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//��������
	int Map[20][10] =
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},

		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},

		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},

		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0}
	};

	//���ж���˹��������
	int Types[7][4] =
	{
		1,3,5,7,	//I
		2,4,5,7,	//Z
		3,5,4,6,	//S
		3,5,4,7,	//T
		2,3,5,7,	//J
		3,5,7,6,	//L
		2,3,4,5		//O
	};

	UPROPERTY(BlueprintReadOnly)
	int Score = 0;//�÷�

	int CurrentTetromino[4][2];//��ǰ����λ��
	int CurrentType;//��ǰ����
	int CurrentVector = 0;//��ǰ�����ʸ�������ڿ�����ת��
	enum AllStatus { Falling, Bottom, NotCreate };//״̬��ö������
	AllStatus Status = NotCreate;
	const int CoolDown = 15.0f;//����CD
	int GameTime = 10000.0f;
	int TimeLeft = CoolDown;

	//AI ��������
	int TempMap[20][10];
	int FinalMap[20][10];
	int TempTetromino[4][2];

	//��������
	void randomOne();
	void showTetromino();
	void showMap();
	void destroyBlock();

	//�����ƶ�����ת
	void moveDown();
	void moveRight();
	void moveLeft();
	void rotate();

	//������
	void clearFullLine();
	void updateMap(int row);

	//AI ����
	void resetMap();
	void setTempMap();
	void setFinalMap();
	void setMap();
	void setAICopyMap();
	void AIOperation();

	//AI ����������
	void calLHandRC(int T[4][2]);
	void AICalculation(int T[4][2]);
	void setTempTetromino();
	void resetCurrentTetromino();

	//AI ����������������ʾ��Ǳ������е����λ�ã�Ѱ��������
	void dfs(int T[4][2]);
	bool checkCollision(int T[4][2]);
	void fall(int Prev[4][2]);


	/*-------------------------------*/
	//PierreDellacheris�㷨�ĺ��Ĳ���
	int LandingHeight;
	int RowCleared;
	int RowTransitions;
	int ColTransitions;
	int HoleSums;
	int WellSums;
	float MaxResult;

	//���Ĳ����ļ��㺯��
	float PierreDellacherieFormule();
	void calRT();
	void calCT();
	void calHS();
	void calWS();
	int calDec(int num);
	int CopyMap[22][12];
};
