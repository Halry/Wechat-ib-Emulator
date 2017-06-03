/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
uint8_t Signin_Count;

                                
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
	System_Startup_Init();
ADC_Get_Value();
	UI_Print_Bat_Stat(UI_BAT_EMPTY);
  while (1)
  {
  /* USER CODE END WHILE */
		
  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}
void Write_Count_to_BKP(unsigned short Count)
{
	
}

