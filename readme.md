#FlyCtrlShield
Lazurite FlyのコントローラシールドからCRANEを制御するためのプログラムです。

#FlyCtrlShield.c
通常のメインプログラムです。

#FlyCtrlShield_booth.c
ブース展示用にDCモータの出力を制御したものです。

#使い方
1. LazuriteFlyとFlyCtrlShield.cの次のパラメータをあわせてください。
	使用する周波数	SUBGHZ_CH		24～61(ただし33は除く)
	PANID			PANID			0x0000-0xFFFF 任意
2. LazuriteFlyのアドレスを設定してください。
	送信先アドレス	HOST_ADDRESS	LazuriteFly側にあわせる


