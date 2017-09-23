//#include "stdafx.h"
#include "App.h"
#include <math.h>
#include <boost/bind.hpp>
extern App*   theApp;

void App::Run() {
	theApp = this;
	client.Run(9002);
}


DataLogic& App::Logic() {
	return logic;
}

tcpclient& App::GetTcpClient() {
	return client;
}

App::App() {

}

App::~App() {

}

CallerFromJava& App::Java() {
	return java;
}