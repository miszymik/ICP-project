#include "map.h"

Map::Map(){}

Map::Map(float new_w, float new_h)
: w(new_w), h(new_h){}

Map::Map(float w, float h, std::vector<Street> s)
: w(w), h(h), streets(s){}

void Map::addStreet(Street s)
{
    auto it = std::find_if(streets.begin(), streets.end(),
        [&s](Street &el) -> bool
        { return s.getID() == el.getID(); });
    
    if (it == streets.end())
        streets.push_back(s);
}

void Map::addStreets(std::vector<Street> v)
{
    for (auto &i : v)
        addStreet(i);
}

void Map::addStation(Station s)
{
    auto it = std::find_if(streets.begin(), streets.end(),
        [&s](Street &el) -> bool
        { return s.getStreetID() == el.getID(); });

    if (it != streets.end()) {
        auto pts = it->getPoints();
        Point P = s.getPoint();

        float res1 = (P.getX() - pts[0].getX()) * (pts[1].getY() - pts[0].getY());
        float res2 = (P.getY() - pts[0].getY()) * (pts[1].getX() - pts[0].getY());

        if (!(res1 == res2 &&
            P.between(pts[0], pts[1]))) {

            std::cerr << "Station does not lay on the street\n";
            exit(1);

            //TODO Vylepsit exception
        }
        stations.push_back(s);
    }
        
    //  TODO exception
}

void Map::addStations(std::vector<Station> v)
{
    for (auto &i : v)
        addStation(i);
}

void Map::createGraph()
{
    g = Graph(streets, stations);
}

void Map::outputGraph()
{
    std::cout << g;
}

int Map::readMap(const char *filename)
{
    int i = 0;
    std::string a, b, c;
    std::ifstream infile;
    infile.open(filename, std::ios::in);

    if (!infile.is_open())
        return -1;

    for (std::string line; std::getline(infile, line); i++) {
        std::istringstream iss(line);
        if (i == 0) {
            if (!(iss >> w >> h)) {
                i = -2;
                break;
            }

        } else {
            std::string name;
            std::vector<float> c(4);
            if (!(iss >> c[0] >> c[1] >> c[2] >> c[3] >> name)) {
                i = -3;
                break;
            }
            else {
                Street s(Point(c[0], c[1]), Point(c[2], c[3]), name);
                addStreet(s);
            }
        }
    }

    infile.close();
    return i;
}


std::map<uint32_t, std::vector<std::pair<Point, uint32_t>>> Map::getGraphMap()
{
	return g.getMap();
}

std::vector<Street> Map::getStreets()
{
	return streets;
}

Map::~Map(){}