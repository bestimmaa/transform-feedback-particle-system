#ifndef __PARTICLES_H
#define __PARTICLES_H

#include <vector>
#include <gloostMath.h>

	struct Particle{
		
		float x,y,z;
		float lifetime;
		float r,g,b,a;
	};

class Particles{

public:

	void createParticles(int numParticles);

	void createStars(int numStars);

	void setLifetime(float time); // when time is set to 1.0 creation and destruction of particles are balanced
	void setSpeed(float speed);


	void updateParticles(gloost::Vector3 direction); // pass a direction for the particle stream

	std::vector<Particle>& getParticles();

	Particles();
	~Particles(void);

private:

	std::vector<Particle> particles;
	float lifetime;
	float spread;
	float particleSpeed;
	int numberOfParticles;
	int numberOfStars;
};
#endif