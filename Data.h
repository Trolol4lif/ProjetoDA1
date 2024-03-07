#ifndef PROJETODA1_DATA_H
#define PROJETODA1_DATA_H

#define CITIES "../dataset/Cities.csv"
#define PIPES "../dataset/Pipes.csv"
#define RESERVOIR "../dataset/Reservoir.csv"
#define STATIONS "../dataset/Stations.csv"


class Data {
    public:
        Data();
        void read_cities();
        void read_pipes();
        void read_reservoir();
        void read_stations();
};


#endif //PROJETODA1_DATA_H
