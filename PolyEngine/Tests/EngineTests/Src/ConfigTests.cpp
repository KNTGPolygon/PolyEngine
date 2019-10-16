#include <pe/Defines.hpp>
#include <catch.hpp>

#include <pe/core/rtti/RTTI.hpp>
#include <Configs/ConfigBase.hpp>
#include <pe/core/math/BasicMath.hpp>
#include <pe/core/math/Vector.hpp>
#include <pe/core/math/Vector2f.hpp>
#include <pe/core/math/Vector2i.hpp>
#include <pe/core/math/Matrix.hpp>
#include <pe/core/math/Color.hpp>
#include <pe/core/math/Quaternion.hpp>
#include <pe/core/UniqueID.hpp>
#include <cstdio>
#include <pe/core/utils/Logger.hpp>
#include <vector>

using namespace Poly;

enum class eConfigTest
{
	VAL_1,
	VAL_2,
	_COUNT
};
REGISTER_ENUM_NAMES(eConfigTest, "val_1", "val_2");

enum class eConfigFlagsTest
{
	VAL_1 = 0x01,
	VAL_2 = 0x02,
	VAL_3 = 0x04,
	VAL_4 = 0x08,
	VAL_5 = 0x10
};

static size_t gFactoryCounterInt = 0;
static size_t gFactoryCounterStdVectorInt = 0;
static size_t gFactoryCounterCustom = 0;


class TestRTTIClass : public RTTIBase
{
	RTTI_DECLARE_TYPE_DERIVED(TestRTTIClass, RTTIBase)
	{
		RTTI_PROPERTY_AUTONAME(Val1, RTTI::ePropertyFlag::NONE);
	}
public:
	TestRTTIClass() {}
	TestRTTIClass(int val) : Val1(val) {}

	bool operator==(const TestRTTIClass& rhs) const { return Val1 == rhs.Val1; }
	bool operator!=(const TestRTTIClass& rhs) const { return !(*this == rhs); }

	friend std::ostream& operator<< (std::ostream& stream, const TestRTTIClass& rhs)
	{
		return stream << "TestRTTIClass[ " << rhs.Val1 << " ]";
	}

	int Val1 = 0;
};
RTTI_DEFINE_TYPE(TestRTTIClass)

class TestRTTIClassPolymorphic : public TestRTTIClass
{
	RTTI_DECLARE_TYPE_DERIVED(TestRTTIClassPolymorphic, TestRTTIClass)
	{
		RTTI_PROPERTY_AUTONAME(Val2, RTTI::ePropertyFlag::NONE);
	}
public:
	TestRTTIClassPolymorphic() {}
	TestRTTIClassPolymorphic(int val, float val2) : TestRTTIClass(val), Val2(val2) {}

	float Val2 = 0.f;
};
RTTI_DEFINE_TYPE(TestRTTIClassPolymorphic)

class TestConfig : public ConfigBase
{
	RTTI_DECLARE_TYPE_DERIVED(TestConfig, ConfigBase)
	{
		RTTI_PROPERTY_AUTONAME(PropBool, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropI8, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropI16, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropI32, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropI64, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropU8, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropU16, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropU32, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropU64, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(Propf32, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(Propf64, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropStr, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropEnum, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropStdVectorInt, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropStdVectorStdVectorInt, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropStdVectorString, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropStdVectorCustom, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropMapIntInt, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropMapStrStdVector, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropMapStrCustom, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropUnorderedMapIntInt, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropUnorderedMapStrStdVector, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropUnorderedMapStrCustom, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropVector, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropVector2f, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropVector2i, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropQuat, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropAngle, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropColor, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropMatrix, RTTI::ePropertyFlag::NONE);

		RTTI_PROPERTY_AUTONAME(PropEnumArrayInt, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropEnumArrayStdVector, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropEnumArrayCustom, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropEnumFlags, RTTI::ePropertyFlag::NONE);

		RTTI_PROPERTY_AUTONAME(PropUUID, RTTI::ePropertyFlag::NONE);

		RTTI_PROPERTY_FACTORY_AUTONAME(PropUniquePtrInt, [](Poly::RTTI::TypeInfo info) { ++gFactoryCounterInt; return new int; }, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_FACTORY_AUTONAME(PropUniquePtrStdVectorInt, [](Poly::RTTI::TypeInfo info) { ++gFactoryCounterStdVectorInt; return new std::vector<int>; }, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_FACTORY_AUTONAME(PropUniquePtrCustom, [](Poly::RTTI::TypeInfo info) { ++gFactoryCounterCustom; return info.CreateInstance(); }, RTTI::ePropertyFlag::NONE);
		RTTI_PROPERTY_AUTONAME(PropUniquePtrPolymorphicCustom, RTTI::ePropertyFlag::NONE);

		RTTI_PROPERTY_AUTONAME(PropRawPtrCustom, RTTI::ePropertyFlag::NONE);
	}
public:
	TestConfig() : ConfigBase("Test", eResourceSource::NONE) 
	{
		PropMapIntInt.insert({1, 2});
		PropMapIntInt.insert({2, 3});

		PropMapStrStdVector.insert({"Val1", { 1, 2, 3 }});
		PropMapStrStdVector.insert({"Val2", { 4, 5, 6 }});

		PropMapStrCustom.insert({"Val1", 1});
		PropMapStrCustom.insert({"Val2", 2});

		PropUnorderedMapIntInt.insert({1, 4});
		PropUnorderedMapIntInt.insert({2, 6});

		PropUnorderedMapStrStdVector.insert({"Val1", { 2, 4, 6 }});
		PropUnorderedMapStrStdVector.insert({"Val2", { 8, 10, 12 }});

		PropUnorderedMapStrCustom.insert({"Val1", 2});
		PropUnorderedMapStrCustom.insert({"Val2", 4});

		PropMatrix.SetRotationZ(60_deg);

		PropUniquePtrInt = std::make_unique<int>(5);
		PropUniquePtrStdVectorInt = std::make_unique<std::vector<int>>(std::vector<int>{ 1,2,3 });
		PropUniquePtrCustom = std::make_unique<TestRTTIClass>(3);
		PropUniquePtrPolymorphicCustom = std::make_unique<TestRTTIClass>(12);

		PropRawPtrCustom = PropUniquePtrCustom.get();
	}

	// Rendering
	bool PropBool = true;
	i8 PropI8 = -1;
	i16 PropI16 = -2;
	i32 PropI32 = -3;
	i64 PropI64 = -4;
	u8 PropU8 = 1;
	u16 PropU16 = 2;
	u32 PropU32 = 3;
	u64 PropU64 = 4;
	float Propf32 = 0.32f;
	double Propf64 = 0.32;
	::pe::core::storage::String PropStr = "Test string";
	eConfigTest PropEnum = eConfigTest::VAL_2;

	std::vector<int> PropStdVectorInt = { 1, 2, 3 };
	std::vector<std::vector<int>> PropStdVectorStdVectorInt = { {1}, { 2, 3 } };
	std::vector<::pe::core::storage::String> PropStdVectorString = { "abc", "efg" };
	std::vector<TestRTTIClass> PropStdVectorCustom = { 1, 2 };

	std::map<int, int> PropMapIntInt;
	std::map<::pe::core::storage::String, std::vector<int>> PropMapStrStdVector;
	std::map<::pe::core::storage::String, TestRTTIClass> PropMapStrCustom;

	std::unordered_map<int, int> PropUnorderedMapIntInt;
	std::unordered_map<::pe::core::storage::String, std::vector<int>> PropUnorderedMapStrStdVector;
	std::unordered_map<::pe::core::storage::String, TestRTTIClass> PropUnorderedMapStrCustom;

	::pe::core::math::Vector PropVector = ::pe::core::math::Vector(1,2,3);
	::pe::core::math::Vector2f PropVector2f = ::pe::core::math::Vector2f(1, 2);
	::pe::core::math::Vector2i PropVector2i = ::pe::core::math::Vector2i(1, 2);
	::pe::core::math::Quaternion PropQuat = ::pe::core::math::Quaternion(::pe::core::math::Vector::UNIT_Z, 60_deg);
	::pe::core::math::Angle PropAngle = 60_deg;
	::pe::core::math::Color PropColor = ::pe::core::math::Color(1, 0.5f, 0.3f, 1);
	::pe::core::math::Matrix PropMatrix;

	::pe::core::utils::EnumArray<int, eConfigTest> PropEnumArrayInt = { { eConfigTest::VAL_1, 1 },{ eConfigTest::VAL_2, 2 } };
	::pe::core::utils::EnumArray<std::vector<int>, eConfigTest> PropEnumArrayStdVector = { { eConfigTest::VAL_1, {1, 2} },{ eConfigTest::VAL_2, {2, 3} } };
	::pe::core::utils::EnumArray<TestRTTIClass, eConfigTest> PropEnumArrayCustom = { { eConfigTest::VAL_1, 1}, { eConfigTest::VAL_2, 2} };
	::pe::core::utils::EnumFlags<eConfigFlagsTest> PropEnumFlags = (eConfigFlagsTest::VAL_1 | eConfigFlagsTest::VAL_3 | eConfigFlagsTest::VAL_5);

	::pe::core::UniqueID PropUUID = ::pe::core::UniqueID::FromString("01234567-89ab-cdef-0123-456789abcdef").value();

	std::unique_ptr<int> PropUniquePtrInt;
	std::unique_ptr<std::vector<int>> PropUniquePtrStdVectorInt;
	std::unique_ptr<TestRTTIClass> PropUniquePtrCustom;
	std::unique_ptr<RTTIBase> PropUniquePtrPolymorphicCustom;

	TestRTTIClass* PropRawPtrCustom;
};
RTTI_DEFINE_TYPE(TestConfig)

void baseValueCheck(const TestConfig& config)
{
	CHECK(config.PropBool == true);
	CHECK(config.PropI8 == -1);
	CHECK(config.PropI16 == -2);
	CHECK(config.PropI32 == -3);
	CHECK(config.PropI64 == -4);
	CHECK(config.PropU8 == 1);
	CHECK(config.PropU16 == 2);
	CHECK(config.PropU32 == 3);
	CHECK(config.PropU64 == 4);
	CHECK(config.Propf32 == Approx(0.32f));
	CHECK(config.Propf64 == Approx(0.32));
	CHECK(config.PropStr == "Test string");
	CHECK(config.PropEnum == eConfigTest::VAL_2);

	REQUIRE(config.PropStdVectorInt.size() == 3);
	CHECK(config.PropStdVectorInt[0] == 1);
	CHECK(config.PropStdVectorInt[1] == 2);
	CHECK(config.PropStdVectorInt[2] == 3);

	REQUIRE(config.PropStdVectorStdVectorInt.size() == 2);
	REQUIRE(config.PropStdVectorStdVectorInt[0].size() == 1);
	CHECK(config.PropStdVectorStdVectorInt[0][0] == 1);
	REQUIRE(config.PropStdVectorStdVectorInt[1].size() == 2);
	CHECK(config.PropStdVectorStdVectorInt[1][0] == 2);
	CHECK(config.PropStdVectorStdVectorInt[1][1] == 3);

	REQUIRE(config.PropStdVectorString.size() == 2);
	CHECK(config.PropStdVectorString[0] == "abc");
	CHECK(config.PropStdVectorString[1] == "efg");

	REQUIRE(config.PropStdVectorCustom.size() == 2);
	CHECK(config.PropStdVectorCustom[0].Val1 == 1);
	CHECK(config.PropStdVectorCustom[1].Val1 == 2);

	REQUIRE(config.PropMapIntInt.size() == 2);
	REQUIRE(config.PropMapIntInt.find(1) != config.PropMapIntInt.end());
	CHECK(config.PropMapIntInt.at(1) == 2);
	REQUIRE(config.PropMapIntInt.find(2) != config.PropMapIntInt.end());
	CHECK(config.PropMapIntInt.at(2) == 3);

	REQUIRE(config.PropMapStrStdVector.size() == 2);
	REQUIRE(config.PropMapStrStdVector.find("Val1") != config.PropMapStrStdVector.end());
	REQUIRE(config.PropMapStrStdVector.at("Val1").size() == 3);
	CHECK(config.PropMapStrStdVector.at("Val1")[0] == 1);
	CHECK(config.PropMapStrStdVector.at("Val1")[1] == 2);
	CHECK(config.PropMapStrStdVector.at("Val1")[2] == 3);
	REQUIRE(config.PropMapStrStdVector.find("Val2") != config.PropMapStrStdVector.end());
	REQUIRE(config.PropMapStrStdVector.at("Val2").size() == 3);
	CHECK(config.PropMapStrStdVector.at("Val2")[0] == 4);
	CHECK(config.PropMapStrStdVector.at("Val2")[1] == 5);
	CHECK(config.PropMapStrStdVector.at("Val2")[2] == 6);

	REQUIRE(config.PropMapStrCustom.size() == 2);
	REQUIRE(config.PropMapStrCustom.find("Val1") != config.PropMapStrCustom.end());
	CHECK(config.PropMapStrCustom.at("Val1").Val1 == 1);
	REQUIRE(config.PropMapStrCustom.find("Val2") != config.PropMapStrCustom.end());
	CHECK(config.PropMapStrCustom.at("Val2").Val1 == 2);

	REQUIRE(config.PropUnorderedMapIntInt.size() == 2);
	REQUIRE(config.PropUnorderedMapIntInt.find(1) != config.PropUnorderedMapIntInt.end());
	CHECK(config.PropUnorderedMapIntInt.at(1) == 4);
	REQUIRE(config.PropUnorderedMapIntInt.find(2) != config.PropUnorderedMapIntInt.end());
	CHECK(config.PropUnorderedMapIntInt.at(2) == 6);

	REQUIRE(config.PropUnorderedMapStrStdVector.size() == 2);
	REQUIRE(config.PropUnorderedMapStrStdVector.find("Val1") != config.PropUnorderedMapStrStdVector.end());
	REQUIRE(config.PropUnorderedMapStrStdVector.at("Val1").size() == 3);
	CHECK(config.PropUnorderedMapStrStdVector.at("Val1")[0] == 2);
	CHECK(config.PropUnorderedMapStrStdVector.at("Val1")[1] == 4);
	CHECK(config.PropUnorderedMapStrStdVector.at("Val1")[2] == 6);
	REQUIRE(config.PropUnorderedMapStrStdVector.find("Val2") != config.PropUnorderedMapStrStdVector.end());
	REQUIRE(config.PropUnorderedMapStrStdVector.at("Val2").size() == 3);
	CHECK(config.PropUnorderedMapStrStdVector.at("Val2")[0] == 8);
	CHECK(config.PropUnorderedMapStrStdVector.at("Val2")[1] == 10);
	CHECK(config.PropUnorderedMapStrStdVector.at("Val2")[2] == 12);

	REQUIRE(config.PropUnorderedMapStrCustom.size() == 2);
	REQUIRE(config.PropUnorderedMapStrCustom.find("Val1") != config.PropUnorderedMapStrCustom.end());
	CHECK(config.PropUnorderedMapStrCustom.at("Val1").Val1 == 2);
	REQUIRE(config.PropUnorderedMapStrCustom.find("Val2") != config.PropUnorderedMapStrCustom.end());
	CHECK(config.PropUnorderedMapStrCustom.at("Val2").Val1 == 4);

	CHECK(config.PropVector == ::pe::core::math::Vector(1, 2, 3));
	CHECK(config.PropVector2f == ::pe::core::math::Vector2f(1, 2));
	CHECK(config.PropVector2i == ::pe::core::math::Vector2i(1, 2));
	CHECK(config.PropQuat == ::pe::core::math::Quaternion(::pe::core::math::Vector::UNIT_Z, 60_deg));
	CHECK(config.PropAngle == 60_deg);
	CHECK(config.PropColor == ::pe::core::math::Color(1, 0.5f, 0.3f, 1));
	::pe::core::math::Matrix testMat;
	testMat.SetRotationZ(60_deg);
	CHECK(config.PropMatrix == testMat);

	CHECK(config.PropEnumArrayInt == ::pe::core::utils::EnumArray<int, eConfigTest>{ { eConfigTest::VAL_1, 1 },{ eConfigTest::VAL_2, 2 } });
	CHECK(config.PropEnumArrayStdVector == ::pe::core::utils::EnumArray<std::vector<int>, eConfigTest>{ { eConfigTest::VAL_1,{ 1, 2 } },{ eConfigTest::VAL_2,{ 2, 3 } } });
	CHECK(config.PropEnumArrayCustom == ::pe::core::utils::EnumArray<TestRTTIClass, eConfigTest>{ { eConfigTest::VAL_1, 1 },{ eConfigTest::VAL_2, 2 } });
	CHECK(config.PropEnumFlags == (eConfigFlagsTest::VAL_1 | eConfigFlagsTest::VAL_3 | eConfigFlagsTest::VAL_5));

	CHECK(config.PropUUID == ::pe::core::UniqueID::FromString("01234567-89ab-cdef-0123-456789abcdef").value());

	CHECK(*config.PropUniquePtrInt == 5);
	CHECK(*config.PropUniquePtrStdVectorInt == std::vector<int>{1,2,3});
	CHECK(config.PropUniquePtrCustom->Val1 == 3);
	TestRTTIClass* ptr = rtti_cast<TestRTTIClass*>(config.PropUniquePtrPolymorphicCustom.get());
	REQUIRE(ptr != nullptr);
	CHECK(ptr->Val1 == 12);

	CHECK(config.PropRawPtrCustom == config.PropUniquePtrCustom.get());
}

TEST_CASE("Config serialization tests", "[ConfigBase]")
{
	REQUIRE(gFactoryCounterInt == 0);
	REQUIRE(gFactoryCounterStdVectorInt == 0);
	REQUIRE(gFactoryCounterCustom == 0);

	{
		TestConfig config;
		// perform first basic save, to ensure file is created, and with proper values
		config.Save();
		// load it
		config.Load();

		REQUIRE(gFactoryCounterInt == 1);
		REQUIRE(gFactoryCounterStdVectorInt == 1);
		REQUIRE(gFactoryCounterCustom == 1);

		baseValueCheck(config);

		// Change values
		config.PropBool = false;
		config.PropI8 = -5;
		config.PropI16 = -6;
		config.PropI32 = -7;
		config.PropI64 = -8;
		config.PropU8 = 5;
		config.PropU16 = 6;
		config.PropU32 = 7;
		config.PropU64 = 8;
		config.Propf32 = 0.64f;
		config.Propf64 = 0.64f;
		config.PropStr = "Test string2";
		config.PropEnum = eConfigTest::VAL_1;

		config.PropStdVectorInt = { 4, 5, 6, 7 };
		config.PropStdVectorStdVectorInt = { { 1, 2 }, { 3 }, {4, 5, 6} };
		config.PropStdVectorString = { "123", "456", "789" };
		config.PropStdVectorCustom = { 3, 4, 5, 6 };

		config.PropMapIntInt.erase(1);
		config.PropMapIntInt[2] = 4;
		config.PropMapIntInt.insert({3, 5});
		config.PropMapIntInt.insert({4, 6});

		config.PropMapStrStdVector.erase("Val1");
		config.PropMapStrStdVector.at("Val2").push_back(9);
		config.PropMapStrStdVector.insert({"Val3", { 7,8 }});

		config.PropMapStrCustom.erase("Val1");
		config.PropMapStrCustom.at("Val2").Val1 = 5;
		config.PropMapStrCustom.insert({"Val3", 3});
		config.PropMapStrCustom.insert({"Val4", 4});

		config.PropUnorderedMapIntInt.erase(1);
		config.PropUnorderedMapIntInt[2] = 8;
		config.PropUnorderedMapIntInt.insert({3, 10});
		config.PropUnorderedMapIntInt.insert({4, 12});

		config.PropUnorderedMapStrStdVector.erase("Val1");
		config.PropUnorderedMapStrStdVector.at("Val2").push_back(18);
		config.PropUnorderedMapStrStdVector.insert({"Val3", { 14, 16 }});

		config.PropUnorderedMapStrCustom.erase("Val1");
		config.PropUnorderedMapStrCustom.at("Val2").Val1 = 10;
		config.PropUnorderedMapStrCustom.insert({"Val3", 6});
		config.PropUnorderedMapStrCustom.insert({"Val4", 8});

		config.PropVector = ::pe::core::math::Vector(4, 5, 6);
		config.PropVector2f = ::pe::core::math::Vector2f(3, 4);
		config.PropVector2i = ::pe::core::math::Vector2i(3, 4);
		config.PropQuat = ::pe::core::math::Quaternion(::pe::core::math::Vector::UNIT_X, 75_deg);
		config.PropAngle = 75_deg;
		config.PropColor = ::pe::core::math::Color(0.5f, 1, 0.8f, 1);
		config.PropMatrix.SetRotationX(75_deg);

		config.PropEnumArrayInt = { { eConfigTest::VAL_1, 3 },{ eConfigTest::VAL_2, 4 } };
		config.PropEnumArrayStdVector = { { eConfigTest::VAL_1,{ 3, 4 } },{ eConfigTest::VAL_2,{ 4, 5 } } };
		config.PropEnumArrayCustom = { { eConfigTest::VAL_1, 3 },{ eConfigTest::VAL_2, 4 } };
		config.PropEnumFlags = (eConfigFlagsTest::VAL_1 | eConfigFlagsTest::VAL_2 | eConfigFlagsTest::VAL_4);

		config.PropUUID = ::pe::core::UniqueID::FromString("abcdef01-2345-6789-abcd-ef0123456789").value();

		config.PropUniquePtrInt = std::make_unique<int>(7);
		config.PropUniquePtrStdVectorInt = std::make_unique<std::vector<int>>(std::vector<int>{ 4, 5, 6 });
		config.PropUniquePtrCustom = std::make_unique<TestRTTIClass>(8);
		config.PropUniquePtrPolymorphicCustom = std::make_unique<TestRTTIClassPolymorphic>(17, 1.5f);

		config.PropRawPtrCustom = config.PropUniquePtrCustom.get();

		// save them
		config.Save();
	}

	{
		TestConfig config;

		// check values again
		baseValueCheck(config);

		// load old values
		config.Load();

		REQUIRE(gFactoryCounterInt == 2);
		REQUIRE(gFactoryCounterStdVectorInt == 2);
		REQUIRE(gFactoryCounterCustom == 2);

		// check values again after load
		CHECK(config.PropBool == false);
		CHECK(config.PropI8 == -5);
		CHECK(config.PropI16 == -6);
		CHECK(config.PropI32 == -7);
		CHECK(config.PropI64 == -8);
		CHECK(config.PropU8 == 5);
		CHECK(config.PropU16 == 6);
		CHECK(config.PropU32 == 7);
		CHECK(config.PropU64 == 8);
		CHECK(config.Propf32 == Approx(0.64f));
		CHECK(config.Propf64 == Approx(0.64));
		CHECK(config.PropStr == "Test string2");
		CHECK(config.PropEnum == eConfigTest::VAL_1);

		REQUIRE(config.PropStdVectorInt.size() == 4);
		CHECK(config.PropStdVectorInt[0] == 4);
		CHECK(config.PropStdVectorInt[1] == 5);
		CHECK(config.PropStdVectorInt[2] == 6);
		CHECK(config.PropStdVectorInt[3] == 7);

		REQUIRE(config.PropStdVectorStdVectorInt.size() == 3);
		REQUIRE(config.PropStdVectorStdVectorInt[0].size() == 2);
		CHECK(config.PropStdVectorStdVectorInt[0][0] == 1);
		CHECK(config.PropStdVectorStdVectorInt[0][1] == 2);
		REQUIRE(config.PropStdVectorStdVectorInt[1].size() == 1);
		CHECK(config.PropStdVectorStdVectorInt[1][0] == 3);
		REQUIRE(config.PropStdVectorStdVectorInt[2].size() == 3);
		CHECK(config.PropStdVectorStdVectorInt[2][0] == 4);
		CHECK(config.PropStdVectorStdVectorInt[2][1] == 5);
		CHECK(config.PropStdVectorStdVectorInt[2][2] == 6);

		REQUIRE(config.PropStdVectorString.size() == 3);
		CHECK(config.PropStdVectorString[0] == "123");
		CHECK(config.PropStdVectorString[1] == "456");
		CHECK(config.PropStdVectorString[2] == "789");

		REQUIRE(config.PropStdVectorCustom.size() == 4);
		CHECK(config.PropStdVectorCustom[0].Val1 == 3);
		CHECK(config.PropStdVectorCustom[1].Val1 == 4);
		CHECK(config.PropStdVectorCustom[2].Val1 == 5);
		CHECK(config.PropStdVectorCustom[3].Val1 == 6);

		REQUIRE(config.PropMapIntInt.size() == 3);
		CHECK(config.PropMapIntInt.find(1) == config.PropMapIntInt.end());
		REQUIRE(config.PropMapIntInt.find(2) != config.PropMapIntInt.end());
		CHECK(config.PropMapIntInt.at(2) == 4);
		REQUIRE(config.PropMapIntInt.find(3) != config.PropMapIntInt.end());
		CHECK(config.PropMapIntInt.at(3) == 5);
		REQUIRE(config.PropMapIntInt.find(4) != config.PropMapIntInt.end());
		CHECK(config.PropMapIntInt.at(4) == 6);

		REQUIRE(config.PropMapStrStdVector.size() == 2);
		CHECK(config.PropMapStrStdVector.find("Val1") == config.PropMapStrStdVector.end());
		REQUIRE(config.PropMapStrStdVector.find("Val2") != config.PropMapStrStdVector.end());
		REQUIRE(config.PropMapStrStdVector.at("Val2").size() == 4);
		CHECK(config.PropMapStrStdVector.at("Val2")[0] == 4);
		CHECK(config.PropMapStrStdVector.at("Val2")[1] == 5);
		CHECK(config.PropMapStrStdVector.at("Val2")[2] == 6);
		CHECK(config.PropMapStrStdVector.at("Val2")[3] == 9);
		REQUIRE(config.PropMapStrStdVector.find("Val3") != config.PropMapStrStdVector.end());
		REQUIRE(config.PropMapStrStdVector.at("Val3").size() == 2);
		CHECK(config.PropMapStrStdVector.at("Val3")[0] == 7);
		CHECK(config.PropMapStrStdVector.at("Val3")[1] == 8);

		REQUIRE(config.PropMapStrCustom.size() == 3);
		CHECK(config.PropMapStrCustom.find("Val1") == config.PropMapStrCustom.end());
		REQUIRE(config.PropMapStrCustom.find("Val2") != config.PropMapStrCustom.end());
		CHECK(config.PropMapStrCustom.at("Val2").Val1 == 5);
		REQUIRE(config.PropMapStrCustom.find("Val3") != config.PropMapStrCustom.end());
		CHECK(config.PropMapStrCustom.at("Val3").Val1 == 3);
		REQUIRE(config.PropMapStrCustom.find("Val4") != config.PropMapStrCustom.end());
		CHECK(config.PropMapStrCustom.at("Val4").Val1 == 4);

		REQUIRE(config.PropUnorderedMapIntInt.size() == 3);
		CHECK(config.PropUnorderedMapIntInt.find(1) == config.PropUnorderedMapIntInt.end());
		REQUIRE(config.PropUnorderedMapIntInt.find(2) != config.PropUnorderedMapIntInt.end());
		CHECK(config.PropUnorderedMapIntInt.at(2) == 8);
		REQUIRE(config.PropUnorderedMapIntInt.find(3) != config.PropUnorderedMapIntInt.end());
		CHECK(config.PropUnorderedMapIntInt.at(3) == 10);
		REQUIRE(config.PropUnorderedMapIntInt.find(4) != config.PropUnorderedMapIntInt.end());
		CHECK(config.PropUnorderedMapIntInt.at(4) == 12);

		REQUIRE(config.PropUnorderedMapStrStdVector.size() == 2);
		CHECK(config.PropUnorderedMapStrStdVector.find("Val1") == config.PropUnorderedMapStrStdVector.end());
		REQUIRE(config.PropUnorderedMapStrStdVector.find("Val2") != config.PropUnorderedMapStrStdVector.end());
		REQUIRE(config.PropUnorderedMapStrStdVector.at("Val2").size() == 4);
		CHECK(config.PropUnorderedMapStrStdVector.at("Val2")[0] == 8);
		CHECK(config.PropUnorderedMapStrStdVector.at("Val2")[1] == 10);
		CHECK(config.PropUnorderedMapStrStdVector.at("Val2")[2] == 12);
		CHECK(config.PropUnorderedMapStrStdVector.at("Val2")[3] == 18);
		REQUIRE(config.PropUnorderedMapStrStdVector.find("Val3") != config.PropUnorderedMapStrStdVector.end());
		REQUIRE(config.PropUnorderedMapStrStdVector.at("Val3").size() == 2);
		CHECK(config.PropUnorderedMapStrStdVector.at("Val3")[0] == 14);
		CHECK(config.PropUnorderedMapStrStdVector.at("Val3")[1] == 16);

		REQUIRE(config.PropUnorderedMapStrCustom.size() == 3);
		CHECK(config.PropUnorderedMapStrCustom.find("Val1") == config.PropUnorderedMapStrCustom.end());
		REQUIRE(config.PropUnorderedMapStrCustom.find("Val2") != config.PropUnorderedMapStrCustom.end());
		CHECK(config.PropUnorderedMapStrCustom.at("Val2").Val1 == 10);
		REQUIRE(config.PropUnorderedMapStrCustom.find("Val3") != config.PropUnorderedMapStrCustom.end());
		CHECK(config.PropUnorderedMapStrCustom.at("Val3").Val1 == 6);
		REQUIRE(config.PropUnorderedMapStrCustom.find("Val4") != config.PropUnorderedMapStrCustom.end());
		CHECK(config.PropUnorderedMapStrCustom.at("Val4").Val1 == 8);

		CHECK(config.PropVector == ::pe::core::math::Vector(4, 5, 6));
		CHECK(config.PropVector2f == ::pe::core::math::Vector2f(3, 4));
		CHECK(config.PropVector2i == ::pe::core::math::Vector2i(3, 4));
		CHECK(config.PropQuat == ::pe::core::math::Quaternion(::pe::core::math::Vector::UNIT_X, 75_deg));
		CHECK(config.PropAngle == 75_deg);
		CHECK(config.PropColor == ::pe::core::math::Color(0.5f, 1, 0.8f, 1));
		::pe::core::math::Matrix testMat;
		testMat.SetRotationX(75_deg);
		CHECK(config.PropMatrix == testMat);

		CHECK(config.PropEnumArrayInt == ::pe::core::utils::EnumArray<int, eConfigTest>{ { eConfigTest::VAL_1, 3 }, { eConfigTest::VAL_2, 4 } });
		CHECK(config.PropEnumArrayStdVector == ::pe::core::utils::EnumArray<std::vector<int>, eConfigTest>{ { eConfigTest::VAL_1, { 3, 4 } }, { eConfigTest::VAL_2,{ 4, 5 } } });
		CHECK(config.PropEnumArrayCustom == ::pe::core::utils::EnumArray<TestRTTIClass, eConfigTest>{ { eConfigTest::VAL_1, 3 }, { eConfigTest::VAL_2, 4 } });
		CHECK(config.PropEnumFlags == (eConfigFlagsTest::VAL_1 | eConfigFlagsTest::VAL_2 | eConfigFlagsTest::VAL_4));

		CHECK(config.PropUUID == ::pe::core::UniqueID::FromString("abcdef01-2345-6789-abcd-ef0123456789").value());

		CHECK(*config.PropUniquePtrInt == 7);
		CHECK(*config.PropUniquePtrStdVectorInt == std::vector<int>{4,5,6});
		CHECK(config.PropUniquePtrCustom->Val1 == 8);
		TestRTTIClassPolymorphic* ptr = rtti_cast<TestRTTIClassPolymorphic*>(config.PropUniquePtrPolymorphicCustom.get());
		REQUIRE(ptr != nullptr);
		CHECK(ptr->Val1 == 17);
		CHECK(ptr->Val2 == 1.5f);

		CHECK(config.PropRawPtrCustom == config.PropUniquePtrCustom.get());
	}

	// remove the config file
	remove("TestConfig.json");
}