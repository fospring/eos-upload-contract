# eos-upload-contract
基于EOS智能合约开发的区块链存证智能合约
合约说明：
系统合约及示例所在目录
uploaddata.cpp
继承eos自合约对象
class uploaddata : public eosio::contract
持久化数据结构table
/// @abi table profiles
    struct profile {
        uint64_t       key;
        string       value;

        uint64_t primary_key() const { return key; }

        EOSLIB_SERIALIZE(profile, (key)(value))
    };

字段	类型	说明
key	uint64_t	持久化数据结构的唯一标识符,底层数据类型需要为uint64_t
value	string	存储的字段，类型为字符串

声明为key：
uint64_t primary_key() const { return key; }
合约对象序列化：
EOSLIB_SERIALIZE(profile, (key)(value))
table常用的方法（https://developers.eos.io/eosio-cpp/reference#multi-index）
类型	参数	功能	前置条件
 emplace	payer-内存ram支付方；
constructor - 郎塔表达式实例化被存储的对象	在table中增加一行（条）记录对象	table被实例化（下同）；
table中不含该key的记录；
payer的ram足够
find	key - table中记录对象的key	查找存在的记录对象	
modify
	itr -指向table中一条记录的迭代器
payer -ram支付方
updater - lambda function表述的更新对象	更新table中存在的一条记录	itr迭代器指向存在的对象；
payer的ram足够
erase	itr-指向要被删除的对象的迭代器	删除表中的一条记录	itr迭代器存在

持久化数据结构名称
基本类型：
[eosio::multi_index](#eosio::multi_index)<uint64_t TableName, typename T, typename... Indices>
合约中命名table：
using profile_table_t = eosio::multi_index<N(profiles),profile>;
结果：名称profiles
类型转换：
N(base32 X) 
描述：Used to generate a compile time uint64_t from the base32 encoded string interpretation of X.把base32字符串转为uin64_t
C++底层定义：
#define N(X)::eosio::string_to_name(#X)

构造函数
explicit uploaddata(account_name self) : contract(self) {}
功能：将合约账户传入合约属性_self
获取合约中table
profile_table_t profile(_self,_self);
验证：
验证方式	验证
require_auth(_self);	必须合约账户本身操作（目前合约中使用）
require_auth(N(ecdowsurecom));	指定某一账户操作而其他账户不能（层使用过）
eosio_assert(uint32_t test,const char * msg)	test的表达式结果为0，中断，回滚；test为1，继续。
msg提示信息
数据类型
account_name：底层数据类型uin64_t