#include <iostream>
#include <queue>

using namespace std;

struct tint
{
	int x;
	int y;
};


int test_queue(queue<tint> _tmp_q)
{
	cout << "test_queue size : " << _tmp_q.size() << '\n';
	_tmp_q.pop(); 



	cout << "test_queue size : " << _tmp_q.size() << '\n';
	cout << "front element tmp : " << _tmp_q.front().x << '\n';

	_tmp_q.push(_tmp_q.front());
	cout << "test_queue size : " << _tmp_q.size() << '\n';

	//cout << "front element : " << q.front() << '\n';
}

int main(){

	// 큐 생성
	queue<int> q;

	// push
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	q.push(5);
	q.push(6);


	// pop
	q.pop();
	q.pop();
	q.pop();


	tint tmp;
	tmp.x = 1; tmp.y = 1;
	queue<tint> qu;
	qu.push(tmp);
	tmp.x = 2; tmp.y = 2;
	qu.push(tmp);
	tmp.x = 3; tmp.y = 3;
	qu.push(tmp);
	test_queue(qu);

//	cout << "queue size : " << q.size() << '\n';
//
//	cout << "front element : " << q.front() << '\n';
//	cout << "back element : " << q.back() << '\n';
//	cout << "queue size : " << q.size() << '\n';
//	cout << "Is it empty? : " << (q.empty() ? "Yes" : "No") << '\n';

	return 0;

}




