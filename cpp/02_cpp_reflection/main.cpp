
#include <map>
#include <iostream>
#include <string>

typedef void* (*PRTCreateObject)(void);

class ClassA;
class ClassB;


class ClassObjectFactory {

public:
	
	static ClassObjectFactory& getInstance();

	void registerClass(std::string key, PRTCreateObject create_func);

	void* getClassByName(std::string key);

private:
	std::map<std::string, PRTCreateObject> map;
	ClassObjectFactory() {};
};

ClassObjectFactory& ClassObjectFactory::getInstance() {
	static ClassObjectFactory instance;
	return instance;
};

void ClassObjectFactory::registerClass(std::string key, PRTCreateObject create_func) {
	map.insert(std::pair<std::string, PRTCreateObject>(key, create_func));
}

void* ClassObjectFactory::getClassByName(std::string key)
{
	auto it = map.find(key);
	if (it != map.end())
	{
		return it->second();
	}
	else
	{
		return nullptr;
	}
}

class BasicClass {
public:
	virtual void print() {
		std::cout << "BasicClass" << std::endl;
	}
};

class ClassA:public BasicClass {

public:
	void print() override {
		std::cout << "printClassA" << std::endl;
	}
};

ClassA* CreateClassAObject(void) {
	return new ClassA();
}

class ClassB :public BasicClass {
	void print() override {
		std::cout << "printClassB" << std::endl;
	}
};

ClassB* CreateClassBObject() {
	return new ClassB();
}

class RegisterAction {
public:
	RegisterAction(std::string key, PRTCreateObject func) {
		ClassObjectFactory::getInstance().registerClass(key, func);
	}
};


int main() {

	RegisterAction a("ClassA", (PRTCreateObject)CreateClassAObject);
	RegisterAction b("ClassB", (PRTCreateObject)CreateClassBObject);

	BasicClass* classA = (BasicClass*)ClassObjectFactory::getInstance().getClassByName("ClassA");
	BasicClass* classB = (BasicClass*)ClassObjectFactory::getInstance().getClassByName("ClassB");

	classA->print();
	classB->print();

	return 0;
}