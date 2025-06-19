
// 单元测试 - TCPSocket封装
extern void UnitTest_TCPSocket();
// 单元测试 - CircleBuffer的多线程读写
extern void UnitTest_CircleBuffer();
// 单元测试 - Msg的Json序列化和反序列化
extern void UnitTest_MessageDefine();
// 单元测试 - 网络模块工作情况
extern void UnitTest_NetworkService();

int main()
{
	// UnitTest_TCPSocket();
	// UnitTest_CircleBuffer();
	// UnitTest_MessageDefine();
	UnitTest_NetworkService();
}
