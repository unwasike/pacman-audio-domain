/*
 * adventures_with_ip.c
 *
 * Main source file. Contains main() and menu() functions.
 */
#include "adventures_with_ip.h"
#include "xil_mmu.h"
#include <stdbool.h>


/* ---------------------------------------------------------------------------- *
 * 									main()										*
 * ---------------------------------------------------------------------------- *
 * Runs all initial setup functions to initialise the audio codec and IP
 * peripherals, before calling the interactive menu system.
 * ---------------------------------------------------------------------------- */
int main(void)
{
	bool restart = true;

	//Configure the IIC data structure
	IicConfig(XPAR_XIICPS_0_DEVICE_ID);

	//Configure the Audio Codec's PLL
	AudioPllConfig();

	//Configure the Line in and Line out ports.
	//Call LineInLineOutConfig() for a configuration that
	//enables the HP jack too.
	AudioConfigureJacks();

	// Audio shared signals
	volatile bool* audio_mute_signal = (volatile bool*) 0x0F000000; // Mute
	Xil_SetTlbAttributes((bool) audio_mute_signal, NORM_NONCACHE);

	volatile bool* pacman_dies_signal = (volatile bool*) 0x0F000001; // Pacman dies unwasike - are these addresses okay?
	Xil_SetTlbAttributes((bool) pacman_dies_signal, NORM_NONCACHE);

	volatile bool* eat_powerup_signal = (volatile bool*) 0x0F000002; // Pacman eats powerup
	Xil_SetTlbAttributes((bool) eat_powerup_signal, NORM_NONCACHE);

	volatile bool* eat_ghost_signal = (volatile bool*) 0x0F000003; // Pacman eats ghost
	Xil_SetTlbAttributes((bool) eat_ghost_signal, NORM_NONCACHE);

	volatile bool* menu_signal = (volatile bool*) 0x0F000004; // Pacman eats ghost
	Xil_SetTlbAttributes((bool) menu_signal, NORM_NONCACHE);



	//Play constant background audio in a loop
	while(1){
		
		while (*menu_signal == true)
		{
			for (int i = 0; i < MENUAUDIO_SAMPLE_SIZE; i++)
			{
				switch (*audio_mute_signal)
				{
				case true:
					//no audio output
					usleep(20);
					break;
				case false:
					Xil_Out32(I2S_DATA_TX_L_REG, menuaudio[i][0] / 4);
					Xil_Out32(I2S_DATA_TX_R_REG, menuaudio[i][1] / 4);
					usleep(20);
					break;
				}
			}
		}

		//Play initial pacman game start audio
		if (restart == true)
		{
			for (int i = 0; i < STARTUPAUDIO_SAMPLE_SIZE; i++)
			{
				switch (*audio_mute_signal)
				{
				case true:
					//no audio output
					usleep(20);
					break;
				case false:
					Xil_Out32(I2S_DATA_TX_L_REG, startupaudio[i][0] / 4);
					Xil_Out32(I2S_DATA_TX_R_REG, startupaudio[i][1] / 4);
					usleep(20);
					break;
				}
			}
			restart = false;
		}

		for (int i = 0; i < PLAYAUDIO_SAMPLE_SIZE; i++)
		{
			// Check all signals

			if (*pacman_dies_signal == true) // Play pacman dying audio then restart with startup audio
			{
				for (int j = 0; j < PACMANDIESAUDIO_SAMPLE_SIZE; j++)
				{
					switch (*audio_mute_signal)
					{
					case true:
						//no audio output
						usleep(20);
						break;
					case false:
						Xil_Out32(I2S_DATA_TX_L_REG, pacmandiesaudio[j][0] / 4);
						Xil_Out32(I2S_DATA_TX_R_REG, pacmandiesaudio[j][1] / 4);
						break;
						usleep(20);
					}
				}
				*pacman_dies_signal = false;
				restart = true;
				break;
			}

			if (*eat_powerup_signal == true) // Power up mode for 10 seconds
			{
				//UNWASIKE do this for duration of power up mode ie 10 seconds? and do we need the breaks statements?
				// powerupaudio is only 10 seconds so 10 loops will last the full 10 seconds
				for (int seconds = 0; seconds < 10; seconds++)
				{
					for (int j = 0; j < POWERUPAUDIO_SAMPLE_SIZE; j++)
					{
						if (*eat_ghost_signal == true) // Eaten a ghost
						{
							for (int k = 0; k < EATGHOSTAUDIO_SAMPLE_SIZE; k++)
							{
								switch (*audio_mute_signal)
								{
								case true:
									//no audio output
									usleep(20);
									break;
								case false:
									Xil_Out32(I2S_DATA_TX_L_REG, eatghost_audio[k][0] / 4);
									Xil_Out32(I2S_DATA_TX_R_REG, eatghost_audio[k][1] / 4);
									usleep(20);
									break;
								}
							}
							*eat_ghost_signal = false;
							break;
						}
						switch (*audio_mute_signal)
						{
						case true:
							//no audio output
							usleep(20);
							break;
						case false:
							Xil_Out32(I2S_DATA_TX_L_REG, powerupaudio[j][0] / 4);
							Xil_Out32(I2S_DATA_TX_R_REG, powerupaudio[j][1] / 4);
							usleep(20);
							break;
						}
					}
				}
				// After 10 seconds
				*eat_powerup_signal = false;
			}

			switch (*audio_mute_signal)
			{
			case true:
				//no audio output
				usleep(20);
				break;
			case false:
				// Play regular play audio
				Xil_Out32(I2S_DATA_TX_L_REG, playaudio[i][0] / 4);
				Xil_Out32(I2S_DATA_TX_R_REG, playaudio[i][1] / 4);
				usleep(20);
				break;
			}
		}
	}
	//menu();
    return 1;
}
