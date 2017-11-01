//#include "stdafx.h"
#include "App.h"
#include <math.h>
#include <boost/bind.hpp>
extern App*   theApp;

void App::Run() {
	theApp = this;
	logic.SetValue(this->_studentIp,this->_smac);
	client.SetRemoteIp(this->_teachetIp);
	client.Run(9002);
}


DataLogic& App::Logic() {
	return logic;
}

tcpclient& App::GetTcpClient() {
	return client;
}

App::App(std::string teacherIp,std::string studentIp,std::string smac) {
	this->_teachetIp = teacherIp;
	this->_studentIp = studentIp;
	this->_smac = smac;
}

App::~App() {

}

CallerFromJava& App::Java() {
	return java;
}