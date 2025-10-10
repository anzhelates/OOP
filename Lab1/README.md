Лабораторна №1

**Лабораторна робота №1**

Завдання:
**Перелік 1:** структури даних та алгоритми.
(****) Графи на основі списку суміжності, матриці суміжності (збереження даних у вершинах та ребрах графів).
Додавання та видалення вершин/ребер. Перевірка на зв’язність графу. Визначення відстані між двома вершинами графу. 

1. Реалізовано графи на основі списку суміжності та матриці суміжності
(шаблонний абстрактний class Graph та підкласи class AdjacencyList, class AdjacencyMatrix);
2. Збереження даних у класах та ребрах: class Vertex (підклас City) та class Edge;
3. Додавання вершин та ребер: методи addVertex та addEdge;
4. Видалення вершин та ребер: removeVertex та removeEdge;
5. Перевірка графу на зв'язність: перевіряється за допомогою isConnected;
6. Визначення відстаней між двома вершинами: реалізовано за допомогою пошуку
найкоротшого шляху (Dijkstra's algorithm);
7. інші алгоритми на графах: додано BFS, DFS, Dijkstra;
8. реалізація загального механізму, що дозволить підключати нові алгоритми: GraphAlgorithm;

**Перелік 2:** класи для опису даних.
Транспортні засоби. Транспортні засоби різних типів – наземні, водні, повітряні.
Середовище, яким вони пересуваються (зокрема, з пунктами відправлення/призначення, дорогами, перешкодами тощо).
Обчислення оптимальних шляхів між заданими пунктами для заданих транспортних засобів. Моделювання руху транспортних засобів.

9. Транспортні засоби різних типів – наземні, водні, повітряні.
Наземний транспорт - class LandVehicle (підклас Car), рейковий - class RailVehicle (підклас Underground), повітряний - class AirVehicle, водний - class WaterVehicle;
Середовище, яким вони пересуваються (зокрема, з пунктами відправлення/призначення, дорогами, перешкодами тощо):
Транспортні засоби пересуваються на ребрах між містами (class City - Vertex data), повний шлях представляє class Route.
Тип та додаткову характеристику дороги представляють enum class RoadType = { ROAD, RAIL, AIR, WATER } та RoadCharacteristic { HIGHWAY, CITY_STREET, DIRT_ROAD, PARK_ROAD, STANDARD, DENSELY_POPULATED_CITY, OTHER }.
Перешкоди задані у вигляді enum class ObstacleCause { TRAFFIC_JAM, ACCIDENT, CONSTRUCTION, WEATHER_STORM, WEATHER_SNOW, WEATHER_WIND, WEATHER_ICE, CUSTOM_DELAY }
10.  Обчислення оптимальних шляхів між заданими пунктами для заданих транспортних засобів. Моделювання руху транспортних засобів.
Для обчислення оптимальних шляхів між заданими пунктами використовуються статичні методи: алгоритми BFS, DFS, Dijkstra’s algorithm.
Час поїздки обчислює метод calculateTravelTime(const Vehicle& vehicle), на час поїздки та швидкість руху транспорту можуть впливати перешкоди. Враховуються швидкісні обмеження та витрати пального.

Вимоги:
1. Не менше 9 класів (або інших подібних типів):
classes: Graph, Vertex, Edge, AdjacencyList, AdjacencyMatrix, GraphAlgorithm, Vehicle, LandVehicle, Car, RailVehicle, Underground, AirVehicle, WaterVehicle, City, Route, enum classes RoadType, RoadCharacteristics, ObstacleCause.

2. Не менше 15 полів (в сумі в усіх реалізованих класах):
Graph (std::vector<TVertex*> m_vertices, std::vector<TEdge*> m_edges, bool m_directed),
Vertex (int m_id, std::string m_name, std::vector<Vehicle*> m_availableVehicles, bool m_active),
Edge (m_source, m_destination, m_distance, m_type, m_characteristic, m_obstacles, m_active),
AdjacencyList (struct AdjEntry (int m_to, TEdge* m_edge), std::vector<std::list<AdjEntry>> m_adjList),
AdjacencyMatrix (std::vector<std::vector<TEdge*>> m_adjMatrix),
Vehicle (std::string m_name, double m_speed, double m_fuelEfficiency),
City (long m_population),
Route (Vertex* m_startVertex = nullptr, Vertex* m_endVertex = nullptr, const Vehicle* m_vehicle = nullptr, std::vector<Edge*> m_path),
enum classes RoadType, RoadCharacteristics, ObstacleCause.

4. Не менше 25 нетривіальних методів або функцій:
Graph (addVertex(TVertex* vertex), addEdge(TEdge* edge), removeVertex(int id), removeEdge(TEdge* edge) isDirected()),
Vertex (addVehicle(Vehicle* vehicle), markInactive(), isActive()),
Edge (addObstacle(Obstacle* obstacle), calculateTravelTime(const Vehicle& vehicle), markInactive(), markActive(), isActive()),
GraphAlgorithm (static std::vector<int> BFS, static std::vector<int> DFS, static std::vector<double> Dijkstra, static bool isConnected),
Vehicle (can use(RoadType type), calculateFuel(double distance), getSpeed(ObstacleCause cause, RoadCharacteristic characteristic)),
City (isDenselyPopulated()),
Route (addEdge(Edge* edge), totalTime(const Vehicle& vehicle), totalDistance(), totalFuel(const Vehicle& vehicle)),
enum classes RoadType, RoadCharacteristics, ObstacleCause.

6. Не менше 2 ієрархій успадкування, хоча б одна з який містить не менше 3 класів:
Graph->AdjacencyList
Graph->AdjacencyMatrix
Vehicle->LandVehicle->Car
Vehicle->RailVehicle->Underground
Vehicle->AirVehicle
Vehicle->WaterVehicle
Vertex->City

8. Не менше 3 незалежних випадків поліморфізму:
Динамічний: віртуальні методи у Graph, Vehicle;
Статичний: шаблонні класи: Graph<TVertex, TEdge>, AdjacencyList<TVertex, TEdge>, AdjacencyMatrix<TVertex, TEdge>, GraphAlgorithm<TVertex, TEdge>,
шаблонні методи BFS, DFS, Dijkstra, isConnected;
Інкапсуляція: усі поля приватні/захищені, доступ можливий лише через геттери/сеттери
Абстракція: абстрактний клас Vehicle, абстрактний шаблонний клас Graph.

Додатково:
Реалізовано unit tests із використанням doctest.h.
