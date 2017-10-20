/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
extern TIM_HandleTypeDef htim4;


const char *System_Version="1.2a";
const char *HW_Ver="0.1a";
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
  System_Startup_Init();
  UI_Main();
  while(1)
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
    }
}
