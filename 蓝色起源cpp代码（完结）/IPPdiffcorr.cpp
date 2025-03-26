int diffcorr(Ipp32fc* uw, int uwlen, Ipp32fc* data, int datalen, int sps, Ipp32f* outdata, int* outdatalen)
{
	//独特码差分，输出uwdiff
	Ipp32fc* uw1 = ippsMalloc_32fc((uwlen-1) * sps);
	Ipp32fc* uw2 = ippsMalloc_32fc((uwlen-1) * sps);
	Ipp32fc* uw1conj = ippsMalloc_32fc((uwlen - 1) * sps);
	Ipp32fc* uwdiff = ippsMalloc_32fc((uwlen - 1) * sps);

	ippsCopy_32fc(uw, uw1, (uwlen-1) * sps);
	ippsCopy_32fc(uw+sps, uw2, (uwlen - 1) * sps);
	ippsConj_32fc(uw1, uw1conj, (uwlen - 1) * sps);
	ippsMul_32fc(uw2, uw1conj, uwdiff, (uwlen - 1) * sps);

	//数据差分，输出datadiff
	Ipp32fc* data1 = ippsMalloc_32fc((datalen - 1) * sps);
	Ipp32fc* data2 = ippsMalloc_32fc((datalen - 1) * sps);
	Ipp32fc* data1conj = ippsMalloc_32fc((datalen - 1) * sps);
	Ipp32fc* datadiff = ippsMalloc_32fc((datalen - 1) * sps);

	ippsCopy_32fc(data, data1, (datalen - 1) * sps);
	ippsCopy_32fc(data + sps, data2, (datalen - 1) * sps);
	ippsConj_32fc(data1, data1conj, (datalen - 1) * sps);
	ippsMul_32fc(data2, data1conj, datadiff, (datalen - 1) * sps);

	//uwdiff翻转
	Ipp32fc* uwdiffflip = ippsMalloc_32fc((uwlen - 1) * sps);
	ippsFlip_32fc(uwdiff, uwdiffflip, (uwlen - 1) * sps);

	//通过卷积相关，输出是corr
	Ipp32fc* corr = ippsMalloc_32fc((uwlen + datalen - 3) * sps);
	ippsConv_32fc(uwdiffflip, (uwlen - 1) * sps, datadiff, (datalen - 1) * sps, corr);

	//归一化用的分母，输出是corrdeno
	Ipp32f* corrdeno = ippsMalloc_32f((uwlen + datalen - 3) * sps);
	Ipp32f* absuwdiffflip = ippsMalloc_32f((uwlen - 1) * sps);
	Ipp32f* absdatadiff = ippsMalloc_32f((datalen - 1) * sps);

	ippsAbs_32fc(uwdiffflip, absuwdiffflip, (uwlen - 1) * sps);
	ippsAbs_32fc(datadiff, absdatadiff, (datalen - 1) * sps);
	ippsConv_32f(absuwdiffflip, (uwlen - 1) * sps, absdatadiff, (datalen - 1) * sps, denocorr);

	//abs corr
	Ipp32f* abscorr = ippsMalloc_32f((uwlen + datalen - 3) * sps);
	ippsAbs_32fc(corr, abscorr, (uwlen + datalen - 3) * sps);

	//out计算
	Ipp32f* out = ippsMalloc_32f((uwlen + datalen - 3) * sps);
	ippsDiv_32f(abscorr, abscorrdeno, out, (uwlen + datalen - 3) * sps);

	//输出
	outdata = out;
	*outdatalen = (uwlen + datalen - 3) * sps;


	//释放内存
	ippFree(uw1);
	ippFree(uw2);
	ippFree(uw1conj);
	ippFree(uwdiff);

	ippFree(data1);
	ippFree(data2);
	ippFree(data1conj);
	ippFree(datadiff);

	ippFree(uwdiffflip);

	ippFree(corr);

	ippFree(corrdeno);
	ippFree(absuwdiffflip);
	ippFree(absdatadiff);

	ippFree(abscorr);

	ippFree(out);

	return 0;
}