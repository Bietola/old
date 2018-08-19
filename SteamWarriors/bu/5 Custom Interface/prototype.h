struct Entity {
	vector<Move> moves;
}

struct Move {
	Action action;
	TextInfo textInfo;
}

//entities.h

namespace ents {
	class CyborgWarrior : public Entity {
		CyborgWarrior();
	}
	class DirtyPirate : public Entity {
		DirtyPirate();
	}
}

//entities.cpp

ents::CyborgWarrior::CyborgWarrior() {
	
	status = {
		{"strength", 10},
		{"toughness", 20}
	};
	
	moves = {
		AttackMove(
			TextInfo("cannon blast",
					 "a ionic cannon blast capable of inflicting considerable...")
			[](const auto& me){
				Effect cannonBlasted;
				cannonBlasted.status = {
					{"strength", 0.75},
					...
				};
				eact::apply(eact::ApplyEffect(me, cannonBlasted));
				eact::apply(eact::ForcedMove(me, Entity::opposite(me.mLastDirection)));
			};
		),
		AttackMove(
			TextInfo("sinner's bite",
					 "works only if there are enough people around to watch...")
			[](const auto& me) {
				unsigned int sacrifices = 0;
				vector<Entity*> targets;
				for(auto& ent : gWord.getEntities()) {
					if(World::distance(ent, me) < 10){
						sacrifices++;
						targets.push_back(*ent);
					}
				}
				if(sacrifices < 10) {
					eact::apply(eact::Transform(me, Demon()));
				} else {
					eact::apply(eact::)
				}
			};
		),
		movs::DinosaurSummon("bill the trex", 10, movs::DinoSummon::Claws)
	};
}
