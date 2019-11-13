#!/usr/bin/python

def palindrome_check(num):
	num_string = str(num)
	num_len = len(num_string)

	for i in range(0, num_len):
#		print str(num) + " : " + str(i) + " " + str(num_len) + " " + num_string[i] + " " + num_string[num_len-(i+1)]
		if num_string[i] != num_string[num_len-(i+1)]:
			return False
	return True


def solution(n,m):
	answer = 0
	for i in range(n,m):
		if palindrome_check(i) == True:
			answer = answer + 1
	
	return answer



print solution(100, 300);
