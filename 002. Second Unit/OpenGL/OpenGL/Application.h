#pragma once
class Application
{
public:
	Application();
	~Application();

	virtual bool startup() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void shutdown() = 0;
};

