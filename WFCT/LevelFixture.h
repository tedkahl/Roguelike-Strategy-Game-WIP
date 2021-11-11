#pragma once
#include "pch.h"
#include "../WFC/Level.h"
#include "../WFC/AIPreference.h"
matrix<char> tOutput{ 7,7, {

'x','D','D','x','x','x','x',
'x','x','x','x','x','x','x',
'x','x','x','x','x','x','x',
'.','.','.','.','x','x','x',
'x','x','x','x','x','x','x',
'x','x','.','.','.','.','x',
'W','x','x','x','x','W','x'} };

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
	//EXPECT_EQ(*(level.units)->active(), 4);
}