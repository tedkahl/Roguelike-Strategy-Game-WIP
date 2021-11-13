#include "Level.h"
Level::Level(std::shared_ptr<ResourceManager<sf::Texture>> tm) :tm_(tm), dcomponents(new SortedDManager<DrawComponent >), units(new UnitManager), entities(new DataManager<Entity>) {
	cout << "dc: " << sizeof(*dcomponents) << endl;
	cout << "uc: " << sizeof(*units) << endl;
	cout << "e: " << sizeof(*entities) << endl;
	cout << "data: " << sizeof(Data<char>::d()) << endl;
	cout << "uc: " << sizeof(Animation) << endl;
	cout << "e: " << sizeof(*entities) << endl;
}



std::optional<sf::Vector2i> Level::getCoords(sf::RenderWindow& window, sf::Vector2i pixel) {
	auto coords = window.mapPixelToCoords(pixel);

	double x_pos = coords.y + (.5 * coords.x);
	double y_pos = coords.y - (.5 * coords.x);
	x_pos = x_pos / sq::square_h - (float)state.board.height() / 2;
	y_pos = y_pos / sq::square_h + (float)state.board.height() / 2;

	std::cout << "xy: " << x_pos << " " << y_pos << std::endl;
	if (0 <= x_pos && x_pos < state.board.width() && 0 <= y_pos && y_pos < state.board.height())
		return sf::Vector2i(static_cast<int>(x_pos), static_cast<int>(y_pos));
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
	auto& view = window.getView();
	sf::FloatRect v(view.getCenter() - view.getSize() / 2.0f, view.getSize());
	dcomponents->sort();
	for (auto& i : *dcomponents) {
		if (i.getSprite().getGlobalBounds().intersects(v))
			i.draw(&window);
	}
}
//renderBatch::~renderBatch() {
//	DCManager.deleteBatch(batch);
//}
//
//renderBatch::renderBatch(int b, std::vector<sf::RenderTexture>&& txt, SortedDManager<DrawComponent>& dcomponents) : batch(b), textures(txt), DCManager(dcomponents) {}

//void Level::displayDJ(dj_map& test) {
//	static std::unique_ptr<renderBatch> b;
//	if (b != nullptr) {
//		b = nullptr;
//		return;
//	}
//	float max_score = 0;
//	int batch = dcomponents->addBatch();
//	for (auto& i : test.map) {
//		max_score = std::max(max_score, i.score);
//	}
//	sf::Font roboto;
//	roboto.loadFromFile("./Roboto/Roboto-Regular.ttf");
//	DrawComponent* obj_dc;
//	std::vector<sf::RenderTexture> spare_textures;
//	for (unsigned y = 0;y < test.map.height();y++) {
//		for (unsigned x = 0;x < test.map.width();x++) {
//			auto normalized_score = std::min(1.0f, 2 * test.map.at(x, y).score / max_score);
//			auto type = normalized_score < .3 ? object_type::MOVESELECT : object_type::ATTACKSELECT;
//			spare_textures.push_back(get_DJ_Square(roboto, normalized_score, *tm_, type));
//			obj_dc = dcomponents->declareNewBatched(spare_textures.back().getTexture(), sf::Vector2f(), 1, sf::IntRect(), batch);
//
//			//std::cout << "Adding target square at " << to_string(sf::Vector2i(i, j) + paths.offset) << std::endl;
//			obj_dc->updateEntityPos(sf::Vector2i(x, y), state.heightmap);
//		}
//	}
//	b = std::make_unique<renderBatch>(batch, std::move(spare_textures), *dcomponents);
//}


//this is pretty goofy. Probably just looping through entities is faster.
Entity* Level::entityClickedOn(const sf::RenderWindow& window, sf::Vector2i coords, sf::Vector2i pixel_, bool prefer_team, int team)
{
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
		cout << "coords " << to_string(parent->coords()) << endl;
		dcp->updateEntityPos(parent->coords(), state.heightmap);
	}
}


unsigned Level::addTargeter(const pathsGrid& paths)
{
	unsigned batch = dcomponents->addBatch();
	DrawComponent* obj_dc;
	for (unsigned i = 0;i < paths.grid.width();i++) {
		for (unsigned j = 0;j < paths.grid.height();j++) {
			if (paths.grid.at(i, j).search == paths.search) {
				auto type = paths.grid.at(i, j).movable ? object_type::MOVESELECT : object_type::ATTACKSELECT;
				if (type == object_type::ATTACKSELECT) {
					assert(paths.grid.at(i, j).attackable);
				}
				obj_dc = getObjDC(*dcomponents, *tm_, type, batch); //add all dcomponents
				//std::cout << "Adding target square at " << to_string(sf::Vector2i(i, j) + paths.offset) << std::endl;
				obj_dc->updateEntityPos(sf::Vector2i(i, j) + paths.offset, state.heightmap);
			}
		}
	}
	return batch;
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
		dcomponents->deactivate(e->dc()->sortVal());
		if (e->uc()) units->deactivate(e->uc()->index());
		entities->deactivate(e->index());
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
			state.board.at(x, y) = Square(terrain_t, dcomponents.get(), dcomponents->declareNew(tm_->get(path), offset, 0, rect), sf::Vector2i(x, y), state.heightmap);
			if (entity_t != object_type::NONE) {
				e = addEntity(entity_t, 2, sf::Vector2i(x, y));
			}
		}
	}
}

