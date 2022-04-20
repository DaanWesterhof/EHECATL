//
// Created by daang on 26-2-2022.
//

#ifndef EHECATL_UPDATERSYSTEM_HPP
#define EHECATL_UPDATERSYSTEM_HPP

namespace EHECATL{
    #define UPDATER_COUNT 10

    class DroneComponent{
        public:
            virtual void update() = 0;
    };

    class Updater{
        DroneComponent* components[UPDATER_COUNT];

        [[noreturn]] void run(){
            while(true){
                for(int i = 0; i < UPDATER_COUNT; i++){
                    components[i]->update();
                }
            }
        }
    }
}

#endif //EHECATL_UPDATERSYSTEM_HPP
