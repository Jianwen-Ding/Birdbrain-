#ifndef ECSMANAGER_HPP
#define ECSMANAGER_HPP

#include <vector>

#include "ComponentReaderBase.hpp"
#include "Scene.hpp"
#include "SceneView.hpp"

// Handles the regular updates/behaviors of the components within a given scene
// Represents a single scene
// Template represents the arguments needed on update
template <typename... Args>
class SceneManager{
    public:
        // Initializes the behaviors at the current game state
        // Can only happen a single time
        void start(){
            // Ensure that the manager has not been started
            if(!m_started){
                // Loops through readers to call their corresponding update implementations
                for (unsigned int readerIter = 0; readerIter < m_readers.size(); readerIter++){
                    m_readers[readerIter]->fullStart();
                }
                m_started = true;
            } else{
                printf("ERROR::START CALLED MULTIPLE TIMES");
            }
        }

        // Inserts a reader that determines the behavior of a range of components
        // Updates the given scene to the corresponding behaviors
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<ComponentReaderBase<Args...>, T>>>
        void insertReader(){
            m_readers.push_back(std::unique_ptr<ComponentReaderBase<Args...>>(new T(m_sceneState)));
        }

        // Updates the given components within the scene according to the corresponding readers
        void update(Args... args){
            // Ensure that the manager has already been started
            if(m_started){
                // Loops through readers to call their corresponding update implementations
                for (unsigned int readerIter = 0; readerIter < m_readers.size(); readerIter++){
                    m_readers[readerIter]->fullUpdate(args...);
                }
            } else{
                printf("ERROR::START HAS NOT BEEN CALLED YET");
            }
        }

        // Constructor
        SceneManager(std::shared_ptr<Scene> insertScene){
            m_sceneState = insertScene;
        }

    protected:
        // Whether the game has been initialized or not
        bool m_started{false};
    
        // Represents the current game state
        std::shared_ptr<Scene> m_sceneState;

        // The readers that define the behavior of the ecs
        std::vector<std::unique_ptr<ComponentReaderBase<Args...>>> m_readers;
};
#endif