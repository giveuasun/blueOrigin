int Modulation::sbpskMod(const char* symbol, int symbolLen, int sps, Ipp32fc* outData, int* outDataLen)
{
	//申请内存
	Ipp32f* phase = ippsMalloc_32f(symbolLen * sps);
	Ipp32f* phasetmp = ippsMalloc_32f(symbolLen * sps + sps + 1);
	Ipp32f* coef = ippsMalloc_32f(sps / 2);

	//写一个全零数组和一个全一数组
	ippsSet_32f(0.0, phase, symbolLen * sps);
	ippsSet_32f(1.0, coef, sps / 2);//卷积用


	Ipp32f init_state = -1;
	for (Ipp32s i = 0; i < symbolLen - 1; i++)
	{
		if (symbol[i] == 0)
		{
			Ipp32f* p = phase;
			ippsSet_32f(init_state,p,sps);
			p = p+sps;
			if (symbol[i + 1] != symbol[i])
			{
				init_state = -1.0 * init_state;
			}
		}
	}

	if (symbol[symbolLen - 1] == 0)
	{
		ippsSet_32f(init_state,p,sps);
	}
	
	ippsConv_32f(phase, symbolLen * sps, coef, sps / 2, phasetmp);





	//ippsCopy_32f(phasetmp + sps / 4, phase, symbolLen * sps);

	ippsMulC_32f_I(2.0*IPP_PI / sps, phase, symbolLen * sps);//数乘
    
    ippsSet_32f(1.0, phasetmp, symbolLen * sps);



    //output
    ippsPolarToCart_32fc(phasetmp, phase, outData, symbolLen * sps);
	
    
    *outDataLen = symbolLen * sps;
	




    //释放内存，结束
	ippsFree(phase);
	ippsFree(phasetmp);
	ippsFree(coef);
	return 1;
}