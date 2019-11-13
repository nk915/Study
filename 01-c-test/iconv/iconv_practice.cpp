#include <stdio.h>
#include <string>
#include <vector>
#include <iconv.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>


std::string ICONV(const char* strInput, const char* sFrom, const char* sTo)
{
	std::string sret;
	iconv_t cd = (iconv_t)(-1);
	char *in = NULL, *out = NULL;
	char *pin = NULL, *pout = NULL;
	char fbuf[40] = {0}, tbuf[40] = {0};
	size_t inl = 0, outl = 0;
	int nret = 0;
	char errbuf[1024] = {0,};

	sret = strInput;
	if((NULL == sFrom) || (NULL == sTo))
	{
		sprintf(errbuf, "ERROR String NULL (src, dst)");
		sret = errbuf;
		return sret;
	}
	if((0 == strlen(sFrom)) || (0 == strlen(sTo)))
	{
		sprintf(errbuf, "ERROR String zero len (src, dst)");
		sret = errbuf;
		return sret;
	}

	snprintf(fbuf, 40, "%s", sFrom);
	snprintf(tbuf, 40, "%s", sTo);

	if(0 == strcmp(fbuf, tbuf))
	{
		sprintf(errbuf, "ERROR NOT strcmp (%s, %s)", fbuf, tbuf);
		sret = errbuf;
		return sret;
	}
	else
	{
		inl = strlen(strInput) + 2;
		in = new char[inl];
		outl = inl * 2;
		out = new char[outl];
		pin = in;
		pout = out;
		memset(in, 0x00, inl);
		memset(out, 0x00, outl);

		snprintf(in, inl, "%s", strInput);
		if(0 > (cd = iconv_open(tbuf, fbuf)))
		{
			sprintf(errbuf, "ERROR %d (%s)",  errno, strerror(errno));
			sret = errbuf;
			delete [] pin;
			delete [] pout;
			return sret;
		}


		if(0 > (nret = iconv(cd, &in, &inl, &out, &outl)))
		{
			sprintf(errbuf, "ERROR %d (%s)",  errno, strerror(errno));
			sret = errbuf;
			delete [] pin;
			delete [] pout;
			iconv_close(cd);
			return sret;
		}

		iconv_close(cd);
		sret = pout;
		if(NULL != pin) { delete [] pin; pin = NULL; }
		if(NULL != pout) { delete [] pout; pout = NULL; }
	}


	return sret;
}




void utf_to_euc_example()
{
	printf("======== [UTF ====>  EUC] ===========");
	std::vector<char> utf;
	utf.resize(1024, 0x00);

	std::string utf_tmp="系统";
	printf("\n\n  [China] encoding change : UTF-8 => EUC  (%s)\n", utf_tmp.data());
	std::copy(utf_tmp.begin(), utf_tmp.end()+1, utf.begin());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-KR",		ICONV(utf.data(), "UTF-8", "EUC-KR"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-CN",		ICONV(utf.data(), "UTF-8", "EUC-CN"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-JP",		ICONV(utf.data(), "UTF-8", "EUC-JP"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "SHIFT_JIS",		ICONV(utf.data(), "UTF-8", "SHIFT_JIS"  ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "LATIN1",		ICONV(utf.data(), "UTF-8", "LATIN1"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "UTF-8",		    ICONV(utf.data(), "UTF-8", "UTF-8"     ).data());


	utf.resize(1024, 0x00);
	utf_tmp="高層マンション";
	printf("\n\n  [Japan] encoding change : UTF-8 => EUC (%s)\n", utf_tmp.data());
	std::copy(utf_tmp.begin(), utf_tmp.end()+1, utf.begin());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-KR",		ICONV(utf.data(), "UTF-8", "EUC-KR"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-CN",		ICONV(utf.data(), "UTF-8", "EUC-CN"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-JP",		ICONV(utf.data(), "UTF-8", "EUC-JP"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "SHIFT_JIS",		ICONV(utf.data(), "UTF-8", "SHIFT_JIS"  ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "LATIN1",		ICONV(utf.data(), "UTF-8", "LATIN1"     ).data());


	utf.resize(1024, 0x00);
	utf_tmp="테스트2";
	printf("\n\n  [Korea] encoding change : UTF-8 => EUC (%s)\n", utf_tmp.data());
	std::copy(utf_tmp.begin(), utf_tmp.end()+1, utf.begin());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-KR",		ICONV(utf.data(), "UTF-8", "EUC-KR"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-CN",		ICONV(utf.data(), "UTF-8", "EUC-CN"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "EUC-JP",		ICONV(utf.data(), "UTF-8", "EUC-JP"     ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "SHIFT_JIS",		ICONV(utf.data(), "UTF-8", "SHIFT_JIS"  ).data());
	printf("\t %s => %s : %s \n", "UTF-8", "LATIN1",		ICONV(utf.data(), "UTF-8", "LATIN1"     ).data());

}

void euc_to_utf_example()
{
	std::vector<char> euc;
	std::string euc_tmp="";
	euc.resize(1024, 0x00);
	
	printf("\n\n======== [EUC ====>  UTF] ===========");


	char ctmp[] = {0xcf, 0xb5, 0xcd, 0xb3, '\0'};
	euc_tmp.clear();
	euc_tmp.empty();
	euc_tmp = std::string(ctmp);
	printf("\n\n  [China] encoding change : EUC => UTF-8  (系统)\n");
	std::copy(euc_tmp.begin(), euc_tmp.end(), euc.begin());
	printf("\t %s => %s : %s \n", "EUC-CN",    "UTF-8",	ICONV(euc.data(), "EUC-CN",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "EUC-KR",    "UTF-8",	ICONV(euc.data(), "EUC-KR",    "UTF-8").data());
	
	
	std::string tmp_dst;
	tmp_dst = ICONV(euc.data(), "EUC-KR",    "UTF-8");
	//std::vector<char> utf_tmp;
	//utf_tmp.resize(1024, 0x00);
	//std::copy(ICONV(euc.data(), "EUC-KR",    "UTF-8").begin(), ICONV(euc.data(), "EUC-KR",    "UTF-8").end(), utf_tmp.begin());


	printf("\t %s ------------ %s => %s => %s : %s \n", ctmp, "EUC-KR",    "UTF-8",	"EUC-KR",	ICONV(tmp_dst.data(), "UTF-8", "EUC-KR").data());
	
	
	
	printf("\t %s => %s : %s \n", "EUC-JP",    "UTF-8",	ICONV(euc.data(), "EUC-JP",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "SHIFT_JIS", "UTF-8",	ICONV(euc.data(), "SHIFT_JIS", "UTF-8").data());
	printf("\t %s => %s : %s \n", "LATIN1",    "UTF-8",	ICONV(euc.data(), "LATIN1",    "UTF-8").data());



	char ctmp2[] = {0x83, 0x65, 0x83, 0x80, '\0'};
	euc_tmp.clear();
	euc_tmp.empty();
	euc.resize(1024, 0x00);
	euc_tmp = std::string(ctmp2);
	printf("\n\n  [Japan] encoding change : EUC => UTF-8 (テム)\n");
	std::copy(euc_tmp.begin(), euc_tmp.end(), euc.begin());
	printf("\t %s => %s : %s \n", "EUC-CN",    "UTF-8",	ICONV(euc.data(), "EUC-CN",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "EUC-KR",    "UTF-8",	ICONV(euc.data(), "EUC-KR",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "EUC-JP",    "UTF-8",	ICONV(euc.data(), "EUC-JP",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "SHIFT_JIS", "UTF-8",	ICONV(euc.data(), "SHIFT_JIS", "UTF-8").data());
	printf("\t %s => %s : %s \n", "LATIN1",    "UTF-8",	ICONV(euc.data(), "LATIN1",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "UTF-8",    "UTF-8",	ICONV(euc.data(), "UTF-8",    "UTF-8").data());


	
	char ctmp3[] = {0xc5, 0xd7, 0xbd, 0xba, 0xc6, 0xae, '\0'};
	euc.resize(1024, 0x00);
	euc_tmp.clear();
	euc_tmp.empty();
	euc_tmp = std::string(ctmp3);
	printf("\n\n  [Korea] encoding change : EUC => UTF-8 (테스트)\n");
	std::copy(euc_tmp.begin(), euc_tmp.end(), euc.begin());
	printf("\t %s => %s : %s \n", "EUC-CN",    "UTF-8",	ICONV(euc.data(), "EUC-CN",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "EUC-KR",    "UTF-8",	ICONV(euc.data(), "EUC-KR",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "EUC-JP",    "UTF-8",	ICONV(euc.data(), "EUC-JP",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "SHIFT_JIS", "UTF-8",	ICONV(euc.data(), "SHIFT_JIS", "UTF-8").data());
	printf("\t %s => %s : %s \n", "LATIN1",    "UTF-8",	ICONV(euc.data(), "LATIN1",    "UTF-8").data());
	printf("\t %s => %s : %s \n", "EUC-KR",    "EUC-KR",	ICONV(euc.data(), "EUC-KR",    "EUC-KR").data());
	printf("\t %s => %s : %s \n", "UTF-8",    "UTF-8",	ICONV(euc.data(), "UTF-8",    "UTF-8").data());


}



int main(int argc, char** argv)
{

	utf_to_euc_example();
	euc_to_utf_example();
	return 0;
}
