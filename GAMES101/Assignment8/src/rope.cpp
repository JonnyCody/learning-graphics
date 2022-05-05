#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

//        Comment-in this part when you implement the constructor
//        for (auto &i : pinned_nodes) {
//            masses[i]->pinned = true;
//        }
        auto interval = (end-start)/ (num_nodes-1);
        for(int i=0;i<num_nodes;++i)
        {
            auto mass = new Mass(start + i*interval, node_mass, false);
            mass->velocity = Vector2D(0, 0);
            masses.push_back(mass);
        }
        for(int i=1;i<num_nodes;++i)
        {
            auto spring = new Spring(masses[i-1], masses[i], k);
            springs.push_back(spring);
        }
        for(auto &i : pinned_nodes)
        {
            masses[i]->pinned = true;
        }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            auto ba = s->m2->position - s->m1->position;
            auto f = s->k*ba.unit()*(ba.norm()-s->rest_length);

            s->m1->forces += f;
            s->m2->forces -= f;
        }

        for (auto &m : masses)
        {
            float k_d = 0.1;
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces+=gravity*m->mass;

                // TODO (Part 2): Add global damping
                m->forces += -k_d*m->velocity;
                auto a = m->forces/m->mass;
                m->velocity += a*delta_t;
                m->position += m->velocity*delta_t;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            auto ba = s->m2->position - s->m1->position;
            auto f = s->k*ba.unit()*(ba.norm()-s->rest_length);

            s->m1->forces += f;
            s->m2->forces -= f;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                Vector2D temp_position = m->position;
                // TODO (Part 3.1): Set the new position of the rope mass
                m->forces += gravity*m->mass;
                auto a = m->forces/m->mass;
                // TODO (Part 4): Add global Verlet damping
                double  damping_factor = 0.00005;
                m->position = m->position +  (1 - damping_factor) * (m->position - m->last_position) + a * delta_t *delta_t;
                m->last_position = temp_position;
            }
            m->forces = Vector2D(0, 0);
        }
    }
}
