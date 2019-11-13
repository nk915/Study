#include <stdio.h>
#include <iconv.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int string_conv(const char* src_encoding, const char* dst_encoding, char[] src_buf, char[] dst_buf, int buf_size=1024);

int main()
{
	char buf[1024] = {0,};
	int in_size, out_size;

	sprintf(buf, "%s", "한글과 유니코드");

	char *input_buf_ptr = ksc_buf; 

	in_size = strlen(ksc_buf);
	
	// EUC-KR UTF-8로
	string_conv("UTF-8", "EUC-KR", );
	
	
//	it = iconv_open("UTF-8", "EUC-KR"); 
//	ret = iconv(it, &input_buf_ptr, &in_size, &output_buf_ptr, &out_size);
//	if (ret < 0)
//	{
//		
//		strerror_r(errno, buf, sizeof(buf) );
//		printf("ret : %d, errno : %d(%s)\n", ret, errno,buf);
//		return(-1);
//	}
//	else
//	{
//		printf("[%s](%d) => [%s][(%d)\n", ksc_buf, in_size, utf_buf, out_size);
//	}
//	iconv_close(it);

	return 0;
}


int string_conv(const char* dst_encoding, const char* src_encoding, char[] src_buf, int buf_size)
{
	iconv_t it;
	int err = 0;

	char dst_buf[1024] = {0,};
	memset(dst_buf, '\0', 1024);

	char* input_buf_ptr = src_buf;
	char* output_buf_ptr = dst_buf;

	in_size = strlen(utf_buf);
	out_size = sizeof(ksc_buf);


	it = iconv_open(dst_encoding, src_encoding);
	ret = iconv(it, &input_buf_ptr, &in_size, &output_buf_ptr, &out_size);

	if (ret < 0)
	{
		err = errno;
		printf("ret : %d, errno : %d\n", ret, errno);
		return(-1);
	}
	else
	{
		printf("[%s](%d) => [%s][(%d)\n", utf_buf, in_size, ksc_buf, out_size);
	}
	iconv_close(it);

	return 1;
}
