#include "pch.h"
#include "../WFC/Level.h"
matrix<char> tOutput{ 6,6, {'x','D','D','x','x','x',
'x','x','x','x','x','x',
'.','.','.','.','x','x',
'x','x','x','x','x','x',
'x','x','.','.','.','.',
'W','x','x','x','x','W' } };

class LevelFixture : public ::testing::Test {
protected:
	std::shared_ptr<ResourceManager<sf::Texture>> tm_ = std::make_shared<ResourceManager<sf::Texture>>();
	Level level;
	LevelFixture() :level(tm_) {}
	void SetUp() override {
		level.setSquares(tOutput);
	}
	void TearDown() override {}
};

TEST_F(LevelFixture, unitsCorrect) {
	EXPECT_EQ(level.units.active(), 4);
}
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}