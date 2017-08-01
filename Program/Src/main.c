/* Includes ------------------------------------------------------------------*/
#include "main.h"
#define SYS_DBG
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
uint8_t Signin_Count;
extern TIM_HandleTypeDef htim4;
                                
                                
const char *System_Version="0.21b";
const char *HW_Ver="1";
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

	System_Startup_Init();
	UI_Main();
	//BT_Init();
	//HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	//HAL_PWR_EnterSTANDBYMode();
  while (1)
  {
  /* USER CODE END WHILE */
		
  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)//every 10ms action
	{	
		Key_Scan();
		ADC_Action(false);
	}
}
