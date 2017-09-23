#ifndef _DATA_QUEUE_H_2017_03_28_LC_
#define _DATA_QUEUE_H_2017_03_28_LC_

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>
#include <boost/make_shared.hpp>

template<typename T>
class DataQueue{
public:
	typedef T* Element;
	void Push(Element element){
		boost::mutex::scoped_lock lock(mtx);
		_datas.push(element);
		if (_datas.size() == 1){
			empty_cond.notify_all();
		}
	}

	Element Pop(){
		boost::mutex::scoped_lock lock(mtx);
		if (_datas.empty())
			empty_cond.wait(lock);
		Element element = _datas.front();
		_datas.pop();
		return  element;
	}
    int  Size(){
        return  _datas.size();
    }
private:
	boost::mutex mtx;
	std::queue<Element> _datas;
	boost::condition_variable empty_cond;
};

#endif