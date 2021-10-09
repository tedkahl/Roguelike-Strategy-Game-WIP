#include "pch.h"

#include "../WFC/ResourceManager.h"
#include "../WFC/SortedDManager.h"
#include "../WFC/Data.h"
//#include "../WFC/Entity.h"
#include "../WFC/DrawComponent.h"
//void DrawComponent::set(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm, unsigned obj_height, const sf::IntRect& rect, int batch)
using namespace std;
class DMFixture :public ::testing::Test {
protected:
	std::shared_ptr<ResourceManager<sf::Texture>> tm = std::make_shared<ResourceManager<sf::Texture>>();
	SortedDManager<DrawComponent> dm;
	matrix<float> heightmap;
	void SetUp() override {
		heightmap.set(10, 10, 0);
		for (int i = 1;i < static_cast<int>(object_type::OBJECT_END);i++) {
			auto [path, offset, rect] = Data<char>::d()->entityinfo[static_cast<object_type>(i)];
			auto newest = dm.declareNew(path, offset, tm, 0, rect);
			/*newest->setSquarePos({i, i}, heightmap);*/
		}
	}
};

TEST_F(DMFixture, t) {
	EXPECT_EQ(dm.active(), 7);
}