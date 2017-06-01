/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
unsigned short Signin_Count;


                                
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
	System_Startup_Init();
OLED_ShowString(0,3,"1.3' OLED TEST");
OLED_R_ShowString(1,1,"1.3' OLED TEST");
ADC_Get_Value();
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
void Start_beacon(uint8_t major,uint8_t minor)
{
	
}
