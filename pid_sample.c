/*---------------------------------------------------------------------
PID制御サンプルコード　Ver1.01 Alpha
pid_sample.c

機能：PID制御のテンプレート
	今回は例としてロボットの左右車輪のモータトルクの制御とします。
	処理の流れは
		トルクセンサからの値を取得
		PID制御演算
		モータの印加電圧（PWM）を設定
	これを制御周期毎に実行する形になります。

作成日：2017/9/10

作成者：Goto Shunichi

備考：float型専用

-----------------------------------------------------------------------*/
#include "pid_control_all_f.h"  //PIDライブラリをインクルード

//モータトルク設定値
#define R_MOTOR_SET_TORQUE		(0.01)		//右モータトルク目標値[N*m]
#define L_MOTOR_SET_TORQUE		(0.03)		//左モータトルク目標値[N*m]

//設定パラメータ
#define PGAIN 		(1.0)		//比例ゲイン
#define TI			(1.0)		//積分時間(0.0禁止)
#define TD			(1.0)		//微分時間

#define DT			(0.01)		//制御周期dt[s]

#define DFF			(0.1)		//不完全微分の微分係数(0.1~0.125)

#define OUTMAX		(100.0)		//PID操作量最大値(今回はPWM値とする[%])
#define OUTMIN		(-100.0)	//PID操作量最小値(今回はPWM値とする[%])
#define DELTA_OUTMAX (5.0)		//PID操作変化量最大値(今回はPWM変化率[%/dt])
#define DELTA_OUTMIN (-5.0)		//PID操作変化量最小値(今回はPWM変化率[%/dt])


//PID制御構造体を定義
PIDParameter_t r_torque_pid;    //右モータトルク
PIDParameter_t l_torque_pid;    //左モータトルク

//周期タイマ実行フラグ(割込みハンドラから操作)
volatile int timer_flag = false;


float GetRightTorqueSensor(void){
	/*
	右トルクセンサからのトルク値を取得します。
	トルク値は具体的には[N*m]などを想定しています。
	電流センサの場合は必要ならばフィルタを掛けてから
	トルク定数などを乗じてトルクに変換します。
	*/
	return(0.0);
}

float GetLeftTorqueSensor(void){
	/*
	左トルクセンサからのトルク値を取得します。
	トルク値は具体的には[N*m]などを想定しています。
	電流センサの場合は必要ならばフィルタを掛けてから
	トルク定数などを乗じてトルクに変換します。
	*/
	return(0.0);
}

void SetRightMotorVoltage(float volt){
	/*
	右モータの印加電圧を設定します。
	今回の例では引数は電圧を
	想定しています。
	最終的にPWM制御するならば
	デューティー比に変換してください。
	*/
}

void SetLeftMotorVoltage(float volt){
	/*
	左モータの印加電圧を設定します。
	今回の例では引数は電圧を
	想定しています。
	最終的にPWM制御するならば
	デューティー比に変換してください。
	*/
}

void TimerHandler(void){
	//制御周期DTの周期割込みハンドラを想定します。
	timer_flag = true;
}


int main(void) {

	//目標値(モータのトルク値)
	float set_r_torque = R_MOTOR_SET_TORQUE;
	float set_l_torque = L_MOTOR_SET_TORQUE;

	//フィードバック値(トルクセンサなど)
	float feedback_r_torque = 0.0;
	float feedback_l_torque = 0.0;

	//制御量（モータ印加電圧値）
	float output_r_volt = 0.0;
	float output_l_volt = 0.0;


	//PID制御構造体初期化
	InitPid(&r_torque_pid);
	InitPid(&l_torque_pid);

	//PIDパラメータ設定(右モータ)
	SetPidGain(&r_torque_pid, PGAIN, TI, TD);
	SetPidDt(&r_torque_pid, DT);
	SetPidDff(&r_torque_pid, DFF);
	SetPidOutlim(&r_torque_pid, OUTMAX, OUTMIN);
	SetPidDeltaoutlim(&r_torque_pid, DELTA_OUTMAX, DELTA_OUTMIN);

	//PIDパラメータ設定(左モータ)
	SetPidGain(&l_torque_pid, PGAIN, TI, TD);
	SetPidDt(&l_torque_pid, DT);
	SetPidDff(&l_torque_pid, DFF);
	SetPidOutlim(&l_torque_pid, OUTMAX, OUTMIN);
	SetPidDeltaoutlim(&l_torque_pid, DELTA_OUTMAX, DELTA_OUTMIN);

	while(1){
		if(timer_flag == true){
			timer_flag = false;

			//フィードバック値(トルク値)を取得
			feedback_r_torque = GetRightTorqueSensor();
			feedback_l_torque = GetLeftTorqueSensor();

			//PID制御演算
			output_r_volt = VResPID(&r_torque_pid, set_r_torque, feedback_r_torque);
			output_l_volt = VResPID(&l_torque_pid, set_l_torque, feedback_l_torque);
			
			//モータ印加電圧（PWM）を設定
			SetRightMotorVoltage(output_r_volt);
			SetLeftMotorVoltage(output_l_volt);
		}
	}	
	return (0);
}