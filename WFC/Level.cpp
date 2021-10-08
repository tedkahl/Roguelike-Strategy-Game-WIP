#include "Level.h"
Level::Level(std::shared_ptr<ResourceManager<sf::Texture>> tm) :tm_(tm) {}

//
//
//Square* getSquare(std::pair<unsigned, unsigned> coords)
//{
//	if (0 <= x_pos && x_pos < state.board.width() && 0 <= y_pos && y_pos < state.board.height())
//		return &state.board.at((unsigned)x_pos, (unsigned)y_pos);
//	return nullptr;
//}


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


void Level::update(sf::Time current)
{
	//cout << current.asMilliseconds() << endl;
	for (auto& i : entities) {
		i.update(current);
	}
	for (auto& i : dcomponents) {
		i.updateAnimation(current);
	}
}

void Level::draw(sf::RenderWindow& window) {

	auto& view = window.getView();
	sf::FloatRect v(view.getCenter() - view.getSize() / 2.0f, view.getSize());
	//std::set<DrawComponent> s;
	//for (const auto& i : dcomponents) {
	//	
	//		s.insert(i);
	//}
	dcomponents.sort();
	/*int count = 0;
	sf::Font roboto;
	roboto.loadFromFile("./Roboto/Roboto-Regular.ttf");*/
	for (const auto& i : dcomponents) {
		/*sf::Text ftext;
		ftext = sf::Text(std::to_string(count), roboto, 16);
		ftext.setPosition(i.getSprite().getPosition() + sf::Vector2f(15, 10));
		ftext.setFillColor(sf::Color::Black);*/
		if (i.getSprite().getGlobalBounds().intersects(v))
			i.draw(&window);
	}
}

Entity* Level::entityClickedOn(const sf::RenderWindow& window, sf::Vector2i coords, sf::Vector2i pixel_, bool prefer_team, int team)
{
	dcomponents.sort();
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
					int dist_sq = sumSq(sf::Vector2i(pixel) - sf::Vector2i(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2));
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


Entity* Level::addEntity(object_type type, sf::Vector2i coords)
{
	UnitComponent* uc = makeUnit(units, 0, type);
	if (uc && state.board.at(coords).unit()) {
		std::cerr << "space already full\n";
		return nullptr;
	}
	DrawComponent* dc = getObjDC(dcomponents, tm_, type);
	Entity* entity = entities.declareNew(type, &dcomponents, dc, uc, coords, state);
	return entity;
}


unsigned Level::addTargeter(const pathsGrid& paths)
{
	unsigned batch = dcomponents.addBatch();
	DrawComponent* obj_dc;
	for (unsigned i = 0;i < paths.grid.width();i++) {
		for (unsigned j = 0;j < paths.grid.height();j++) {
			if (paths.grid.at(i, j).search == paths.search) {
				auto type = paths.grid.at(i, j).attack_only ? object_type::ATTACKSELECT : object_type::MOVESELECT;
				obj_dc = getObjDCBatched(dcomponents, tm_, type, batch); //add all dcomponents
				//std::cout << "Adding target square at " << to_string(sf::Vector2i(i, j) + paths.offset) << std::endl;
				obj_dc->updateEntityPos(sf::Vector2i(i, j) + paths.offset, state.heightmap);
			}
		}
	}
	return batch;
}


void Level::removeTargeter(unsigned batch)
{
	dcomponents.deleteBatch(batch);
}


void Level::killUnit(UnitComponent* killer, Entity* target)
{
	removeEntity(target);
}

bool Level::removeEntity(Entity* e)
{
	if (state.board.at(e->getPos()).removeE(e)) {
		dcomponents.deactivate(e->dc()->sortVal());
		if (e->uc()) units.deactivate(e->uc()->index());
		entities.deactivate(e->index());
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
			auto [path, offset, rect] = Data<char>::d()->squareinfo.at(terrain_t);
			state.heightmap.at(x, y) = offset.y;
			state.board.at(x, y) = Square(terrain_t, &dcomponents, dcomponents.declareNew(path, offset, tm_, 0, rect), sf::Vector2i(x, y), state.heightmap);
			if (entity_t != object_type::NONE) {
				e = addEntity(entity_t, sf::Vector2i(x, y));
			}
		}
	}
}

