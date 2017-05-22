/*
 * mathcoord.h
 *
 *  Created on: 02.07.2015
 *      Author: Admin
 */

#ifndef MATHCOORD_H_
#define MATHCOORD_H_

double ConvertPixelToAng(uint16 pixel);
int16	ConvertAngToPixel(double Ang,double focus);
void	NormalizePixelT1(void);
void	NormalizePixelT2(void);
void SetFieldAnalysis(uint16);

#endif /* MATHCOORD_H_ */

