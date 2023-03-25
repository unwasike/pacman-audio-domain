/*
 * adventures_with_ip.c
 *
 * Main source file. Contains main() and menu() functions.
 */
#include "adventures_with_ip.h"


/* ---------------------------------------------------------------------------- *
 * 									main()										*
 * ---------------------------------------------------------------------------- *
 * Runs all initial setup functions to initialise the audio codec and IP
 * peripherals, before calling the interactive menu system.
 * ---------------------------------------------------------------------------- */
int main(void)
{
	//Configure the IIC data structure
	IicConfig(XPAR_XIICPS_0_DEVICE_ID);

	//Configure the Audio Codec's PLL
	AudioPllConfig();

	//Configure the Line in and Line out ports.
	//Call LineInLineOutConfig() for a configuration that
	//enables the HP jack too.
	AudioConfigureJacks();

	//Play initial pacman game start audio
	for (int i = 0; i < STARTUPAUDIO_SAMPLE_SIZE; i++)
	{

		Xil_Out32(I2S_DATA_TX_L_REG, startupaudio[i][0] / 4);
		Xil_Out32(I2S_DATA_TX_R_REG, startupaudio[i][1] / 4);

		usleep(20);
	}

	//Play constant background audio in a loop
	while(1){

		for (int i = 0; i < PLAYAUDIO_SAMPLE_SIZE; i++)
		{

			Xil_Out32(I2S_DATA_TX_L_REG, playaudio[i][0] / 4);
			Xil_Out32(I2S_DATA_TX_R_REG, playaudio[i][1] / 4);

			usleep(20);
		}
	}
	//menu();
    return 1;
}
