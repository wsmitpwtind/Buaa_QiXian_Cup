/**************************************************************
 * 
 * @brief
   ZIN-7Ì×¼þ
	 ·É¿Ø°®ºÃÈº551883670
	 ÌÔ±¦µØÖ·£ºhttps://shop297229812.taobao.com/shop/view_shop.htm?mytmenu=mdianpu&user_number_id=2419305772
 ***************************************************************/
 
#include "kalman.h"
#include <math.h>
/*****************************************************************************************
 * ¿¨¶ûÂü
 * @param[in] 
 * @param[out] 
 * @return     
 ******************************************************************************************/
void kalman_1(struct _1_ekf_filter *ekf,float input)  //Ò»Î¬¿¨¶ûÂü
{
	ekf->Now_P = ekf->LastP + ekf->Q;
	ekf->Kg = ekf->Now_P / (ekf->Now_P + ekf->R);
	ekf->out = ekf->out + ekf->Kg * (input - ekf->out);
	ekf->LastP = (1-ekf->Kg) * ekf->Now_P ;
}
