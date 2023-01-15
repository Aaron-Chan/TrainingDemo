
#include <map>
#include <iostream>
#include <string>
#include <typeinfo>

template<typename T>
void f(T&& param) {
	std::cout << "f the value is " << param << " typeof(param):" << typeid(param).name()<<" typeof(T):" <<typeid(T).name() << std::endl;
}

template <typename T>
void func(T& param)
{
	std::cout << "func the value is " << param << " typeof(param):" << typeid(param).name()<<" typeof(T):" <<typeid(T).name() << std::endl;
}

template <typename T>
void function(T&& param)
{
	std::cout << "function the value is " << param << " typeof(param):" << typeid(param).name()<<" typeof(T):" <<typeid(T).name() << std::endl;
}

//move方法的实现
// FUNCTION TEMPLATE move
// template <class _Ty>
// _NODISCARD constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable
// 	return static_cast<remove_reference_t<_Ty>&&>(_Arg);
// }


//remove_reference的定义
// STRUCT TEMPLATE remove_reference
// template <class _Ty>
// struct remove_reference {
// 	using type                 = _Ty;
// 	using _Const_thru_ref_type = const _Ty;
// };
//
// template <class _Ty>
// struct remove_reference<_Ty&> {
// 	using type                 = _Ty;
// 	using _Const_thru_ref_type = const _Ty&;
// };
//
// template <class _Ty>
// struct remove_reference<_Ty&&> {
// 	using type                 = _Ty;
// 	using _Const_thru_ref_type = const _Ty&&;
// };

//左值和右值
void leftAndRightValue()
{
	int && a = 5;  // 正确，5会被直接存放在寄存器中，所以它是右值
	int b = 10;
	// int && c = b;  // 错误，b在内存中有空间，所以是左值；右值的引用不能绑定到左值上
	
	int && d = b + 5; // 正确，虽然 b 在内存中，但 b+5 的结果放在寄存器中，它没有在内存中分配空间，因此是右值 
	//int && e = a; 错误 无法将右值引用绑定到左值上
	int && e = std::move(a);
}

//通用引用  意思是auto&& 跟 T&& 是一样的  既能够接受左值又能接受右值
void commonReference()
{
	int a = 123;
	auto && b = 5;   //通用引用，可以接收右值

	// int && c = a;    //错误，右值引用，不能接收左值

	auto && d = a;   //通用引用，可以接收左值

	// const auto && e = a; //错误，加了const就不再是通用引用了

	int &g = a;
	f(a);         //通用引用，可以接收左值
	f(10);        //通用引用，可以接收右值
	f(g);        //通用引用，可以接收左值
}

void templateType()
{
	int x = 10;         // x是int
	int & rr = x;       // rr是 int &
	const int cx = x;   // cx是const int
	const int& rx = x;  // rx是const int &
	int *pp = &x;       // pp是int *

	//下面是传值的模板，由于传入参数的值不影响原值，所以参数类型退化为原始类型
	f(x);               // T是int
	f(cx);              // T是int
	f(rx);              // T是int
	f(rr);              // T是int
	f(pp);              // T是int*，指针比较特殊，直接使用

	//下面是传引用模板, 如果输入参数类型有引用，则去掉引用;如果没有引用，则输入参数类型就是T的类型
	func(x);            // T为int
	func(cx);           // T为const int
	func(rx);           // T为const int
	func(rr);           // T为int
	func(pp);           // T是int*，指针比较特殊，直接使用

	//下面是通用引用模板，与引用模板规则一致
	function(x);        // T为int&
	function(5);        // T为int
}

int main() {
	templateType();
	



	return 0;
}