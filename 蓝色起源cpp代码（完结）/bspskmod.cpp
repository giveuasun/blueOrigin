int Modulation::sbpskMod(const char* symbol, int symbolLen, int sps, Ipp32fc* outConstell, int& outLen)
{
	Ipp32f* phase = ippsMalloc_32f(symbolLen * sps);
	Ipp32f* phasetmp = ippsMalloc_32f(symbolLen * sps + sps + 1);
	Ipp32f* coef = ippsMalloc_32f(sps / 2);
	ippsSet_32f(1.0, phase, symbolLen * sps);
	ippsSet_32f(1.0, coef, sps / 2);
	float init_state = -1;
	for (int i = 0; i < symbolLen - 1; i++)
	

	for (int j = 0; j < sps; j++)
	{
		if (symbol[symbolLen - 1] == 0)
		{
			phase[(symbolLen - 1) * sps + j] = init_state;
		}
	}
	ippsConv_32f(phase, symbolLen * sps, coef, sps / 2, phasetmp);
	ippsCopy_32f(phasetmp + sps / 4, phase, symbolLen * sps);
	outLen = symbolLen * sps;
	ippsFree(phase);
	ippsFree(phasetmp);
	ippsFree(coef);
	return 1;
}