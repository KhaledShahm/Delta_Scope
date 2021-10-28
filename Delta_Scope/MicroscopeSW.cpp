#include "MicroscopeSW.h"


MicroscopeSW::MicroscopeSW(const unsigned long CAM_ID, const unsigned long ulPortNo, const unsigned long ulBaundrate)
{
	init(CAM_ID, ulPortNo, ulBaundrate);
}

void MicroscopeSW::init(const unsigned long CAM_ID, const unsigned long ulPortNo, const unsigned long ulBaundrate)
{
	HW_Scope.init(ulPortNo, ulBaundrate);
	CS.init(CAM_ID, 0);
}

void MicroscopeSW::get_image()
{
	
}