#pragma once

namespace tsm
{

// Determines if two Collideable bodies have collided. If they have it emits a
// CollisionEvent. This is used by ExplosionSystem to create explosion
// particles, but it could be used by a SoundSystem to play an explosion
// sound, etc..
//
// Uses a fairly rudimentary 2D partition system, but performs reasonably well.
	class Collision : public ex::System<Collision> {
		static const int PARTITIONS = 200;

		struct Candidate {
			sf::Vector2f position;
			float radius;
			ex::Entity entity;
		};

	public:
		explicit Collision(sf::RenderTarget &target) : size(target.getSize()) {
			size.x = size.x / PARTITIONS + 1;
			size.y = size.y / PARTITIONS + 1;
		}

		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
			reset();
			collect(es);
			collide(events);
		};

	private:
		std::vector<std::vector<Candidate>> grid;
		sf::Vector2u size;

		void reset() {
			grid.clear();
			grid.resize(size.x * size.y);
		}

		void collect(ex::EntityManager &entities) {
			entities.each<tcp::Body, tcp::Collideable>([this](ex::Entity entity, tcp::Body &body, tcp::Collideable &collideable) {
				unsigned int
				left = static_cast<int>(body.position.x - collideable.radius) / PARTITIONS,
				top = static_cast<int>(body.position.y - collideable.radius) / PARTITIONS,
				right = static_cast<int>(body.position.x + collideable.radius) / PARTITIONS,
				bottom = static_cast<int>(body.position.y + collideable.radius) / PARTITIONS;
				Candidate candidate {body.position, collideable.radius, entity};
				unsigned int slots[4] = {
					left + top * size.x,
					right + top * size.x,
					left  + bottom * size.x,
					right + bottom * size.x,
				};
				grid[slots[0]].push_back(candidate);
				if (slots[0] != slots[1]) grid[slots[1]].push_back(candidate);
				if (slots[1] != slots[2]) grid[slots[2]].push_back(candidate);
				if (slots[2] != slots[3]) grid[slots[3]].push_back(candidate);
			});
		}

		void collide(ex::EventManager &events) {
			for (const std::vector<Candidate> &candidates : grid) {
				for (const Candidate &left : candidates) {
					for (const Candidate &right : candidates) {
						if (left.entity == right.entity) continue;
						if (collided(left, right))
							events.emit<tev::Collision>(left.entity, right.entity);
					}
				}
			}
		}

		float length(const sf::Vector2f &v) {
			return std::sqrt(v.x * v.x + v.y * v.y);
		}

		bool collided(const Candidate &left, const Candidate &right) {
			return length(left.position - right.position) < left.radius + right.radius;
		}
	};
}