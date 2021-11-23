#include "Level.h"
#define TEST
Level::Level(std::unique_ptr<ResourceManager<sf::Texture>>&& tm, std::unique_ptr<ResourceManager<sf::Font>>&& fm) :tm_(std::move(tm)), fm_(std::move(fm)), dcomponents(new SortedDManager<DrawComponent >), units(new UnitManager), entities(new DataManager<Entity>) {
	cout << "dc: " << sizeof(*dcomponents) << endl;
	cout << "uc: " << sizeof(*units) << endl;
	cout << "e: " << sizeof(*entities) << endl;
	cout << "data: " << sizeof(Data<char>::d()) << endl;
	cout << "uc: " << sizeof(Animation) << endl;
	cout << "e: " << sizeof(*entities) << endl;
	cout << "tm: " << sizeof(*tm) << endl;
}

std::optional<sf::Vector2i> Level::getCoords(sf::RenderWindow& window, sf::Vector2i pixel) {
	auto pos = gridPosFromScreenCoords(window.mapPixelToCoords(pixel), state.board.height());

	std::cout << "xy: " << pos.x << " " << pos.y << std::endl;
	if (0 <= pos.x && pos.x < state.board.width() && 0 <= pos.y && pos.y < state.board.height())
		return sf::Vector2i(pos);
	return std::nullopt;
}


bool Level::update(sf::Time current)
{
	//cout << current.asMilliseconds() << endl;
	bool block = false;
	for (auto& i : *entities) {
		block = block || i.update(current);
	}
	for (auto& i : *dcomponents) {
		i.updateAnimation(current);
	}
	return block;
}

void Level::draw(sf::RenderWindow& window) {
#ifdef BENCHMARK
	static long double total = 0;
	static int count = 0;
	static int max;
	sf::Clock c;
#endif
	auto& view = window.getView();
	sf::FloatRect v(view.getCenter() - view.getSize() / 2.0f, view.getSize());
	dcomponents->sort();
	for (auto& i : *dcomponents) {
		//if (i.getSprite().getGlobalBounds().intersects(v))
		i.draw(&window);
	}
#ifdef BENCHMARK
	auto elapsed = c.getElapsedTime().asMicroseconds();
	total += elapsed;
	count++;
	max = std::max(max, static_cast<int>(elapsed));
	//cout << total / count << endl;
	if (count > 200) {
		total = 0;
		count = 0;
	}
#endif
	sf::Font& roboto = fm_->get("./Roboto/Roboto-Regular.ttf");
	auto ftext = sf::Text("", roboto, 16);
	ftext.setFillColor(sf::Color::Black);
	for (int y = 0;y < test_display_nums.height();y++) {
		for (int x = 0;x < test_display_nums.width();x++) {
			ftext.setPosition(squarePosFromCoords({ x,y }, state.board.width()) + sf::Vector2f{ 20.f, 10.f });
			ftext.setString(std::to_string(test_display_nums.at(x, y)));
			window.draw(ftext);
		}
	}
}

void Level::displayDJ(matrix<float>&& test) {
	static int batch = -1;
	if (batch != -1) {
		dcomponents->deleteBatch(batch);
	}

	float max_score = 0;
	batch = dcomponents->addBatch();
	for (auto& i : test) {
		max_score = std::max(max_score, i);
	}

	DrawComponent* obj_dc;
	test_display_nums.resize(state.board.width(), state.board.height());
	for (unsigned y = 0;y < test.height();y++) {
		for (unsigned x = 0;x < test.width();x++) {
			auto normalized_score = test.at(x, y) / max_score;

			auto search = Data<char>::d()->entityinfo.find(object_type::DEFAULTSELECT);
			assert(search != Data<char>::d()->entityinfo.end());
			auto& [path, offset, rect] = search->second;

			obj_dc = dcomponents->declareNewBatched(tm_->get(path), sf::Vector2f(), 2, sf::IntRect(), batch);
			obj_dc->setColor({ static_cast<uint8_t>(255 * normalized_score), 0,static_cast<uint8_t>(255 * (1 - normalized_score)), 150 });

			//std::cout << "Adding target square at " << to_string(sf::Vector2i(i, j) + paths.offset) << std::endl;
			obj_dc->updateEntityPos(sf::Vector2i(x, y), state.heightmap);
			test_display_nums.at(x, y) = static_cast<int>(normalized_score * 100);
		}
	}
}


//this is pretty goofy. Probably just looping through entities is faster.
Entity* Level::entityClickedOn(const sf::RenderWindow& window, sf::Vector2i coords, sf::Vector2i pixel_, bool prefer_team, int team)
{
#ifdef TEST
	for (auto& i : *units) {
		assert(i.getPos() == i.getOwner()->getPos());
		assert(i.getPos() == i.getOwner()->dc()->coords());
		assert(i.getPos() == i.getOwner()->getOwner()->getPos());
	}
#endif
	sf::Vector2f pixel = window.mapPixelToCoords(pixel_);
	std::vector<sf::Vector2i> cols{ sf::Vector2i(0,0), sf::Vector2i(1,0), sf::Vector2i(0,1) };

	Entity* clicked = nullptr;
	int min_dist_sq = 1000000;
	sf::Vector2i start = coords;
	for (int j = 0;j < 4;j++) {
		for (auto& i : cols)
		{
			if (on_board(start + i, state.board) && (state.board.at(start + i).unit()))
			{
				std::cout << "unit found" << std::endl;
				Entity* u = state.board.at(start + i).unit();
				assert(u != nullptr);
				//if prefer_ally is set, choose the sprite of the correct team

				if (prefer_team && clicked) {
					bool is_ally_1 = (clicked->uc()->team() == team);
					bool is_ally_2 = (u->uc()->team() == team);
					if (is_ally_1 && !is_ally_2) continue;
					if (is_ally_2 && !is_ally_1) {
						clicked = u;
						continue;
					}
				}

				//else do this nonsense
				sf::FloatRect bounds = u->dc()->getSprite().getGlobalBounds();
				if (bounds.contains(pixel)) {
					int dist_sq = sumSq(sf::Vector2i(pixel) - getCenter(bounds));
					if ((dist_sq - min_dist_sq) < 300) { //choose the closer sprite unless the click is much centered on the farther one.
						clicked = u;
						min_dist_sq = dist_sq;
					}
				}
			}
		}
		start += sf::Vector2i(1, 1);
	}

	return clicked;
}


Entity* Level::addEntity(object_type type, int team, sf::Vector2i coords)
{
	UnitComponent* uc = nullptr;
	assert(on_board(coords, state.board));
	if (isUnitOrObj(type)) {
		if (state.board.at(coords).unit()) {
			std::cerr << "space already full\n";
			return nullptr;
		}
		else {
			uc = makeUnit(*units, team, type);
		}
	}
	DrawComponent* dc = getObjDC(*dcomponents, *tm_, type);
	Entity* entity = entities->declareNew(type, dcomponents.get(), dc, uc, coords, state);
	return entity;
}

Entity* Level::addEntityTest(object_type type, int team, sf::Vector2i coords)
{
	UnitComponent* uc = nullptr;
	assert(on_board(coords, state.board));
	if (isUnit(type)) {
		if (state.board.at(coords).unit()) {
			std::cerr << "space already full\n";
			return nullptr;
		}
		else {
			uc = makeUnit(*units, team, type);
		}
	}
	DrawComponent* dc = getObjDC(*dcomponents, *tm_, type);
	Entity* entity = entities->declareNew(type, dcomponents.get(), dc, uc, coords, state);
	addChildDC(object_type::POISON, dc);
	return entity;
}

void Level::addChildDC(object_type type, DrawComponent* parent)
{
	auto search = Data<char>::d()->entityinfo.find(type);
	if (search != Data<char>::d()->entityinfo.end()) {
		auto& [path, offset, rect] = search->second;
		auto dcp = dcomponents->declareNew(tm_->get(path), parent->getOffset(), 51, rect);
		dcp->updateEntityPos(parent->coords(), state.heightmap);
	}
}

void Level::removeBatch(unsigned batch)
{
	dcomponents->deleteBatch(batch);
}


void Level::killUnit(UnitComponent* killer, Entity* target)
{
	removeEntity(target);
}

bool Level::removeEntity(Entity* e)
{
	if (state.board.at(e->getPos()).removeE(e)) {
		dcomponents->deactivateAll(e->dc()->sortVal());
		if (e->uc()) units->deactivate(e->uc()->index());
		entities->deactivate(e->index());
#ifdef TEST
		for (auto& i : *units) {
			assert(i.getPos() == i.getOwner()->getPos());
			assert(i.getPos() == i.getOwner()->getOwner()->getPos());
		}
#endif
		return true;
	}
	return false;
}

void Level::setSquares(matrix<char>& WFCOutput)
{
	state.board.set(WFCOutput.width(), WFCOutput.height(), Square());
	state.heightmap.set(WFCOutput.width(), WFCOutput.height(), 0);
	Entity* e;
	for (unsigned y = 0;y < WFCOutput.height();y++) {
		for (unsigned x = 0;x < WFCOutput.width();x++) {
			char val = WFCOutput.at(x, y);
			auto [terrain_t, entity_t] = Data<char>::d()->glyphs.at(val);
			auto& [path, offset, rect] = Data<char>::d()->squareinfo.at(terrain_t);
			state.heightmap.at(x, y) = offset.y;
			state.board.at(x, y) = Square(terrain_t, dcomponents->declareNew(tm_->get(path), offset, 0, rect), sf::Vector2i(x, y), state.heightmap);
			if (entity_t != object_type::NONE) {
				e = addEntity(entity_t, 2, sf::Vector2i(x, y));
			}
		}
	}
}

