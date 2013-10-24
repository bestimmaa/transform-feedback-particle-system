
#include "Particles.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <gloostMath.h>

void Particles::createParticles(int numParticles){

	numberOfParticles = numParticles;

	particles.clear();

	for(int i=0; i < numParticles ;i++){

		struct Particle myParticle = {0.0,//pos x
									  0.0,//	y
									  0.0,//	z
									  0.0,//lifetime
									  0.0,//r
									  1.0,//g
									  0.0,//b
									  1.0,};//a
	
		particles.push_back(myParticle);

	}

}

void Particles::createStars(int numStars){

	numberOfParticles = numStars;

	for(int i=0; i < numberOfParticles ;i++){

	struct Particle myParticle = {static_cast<float>(gloost::getRandomMinMax(-50,50)),//pos x
								  static_cast<float>(gloost::getRandomMinMax(-50,50)),//	y
								  static_cast<float>(gloost::getRandomMinMax(-50,50)),//	z
									  1.0,//lifetime
									  1.0,//r
									  1.0,//g
									  1.0,//b
									  1.0,};//a
	
		particles.push_back(myParticle);

	}
	
}

void Particles::updateParticles(gloost::Vector3 direction){


	float random = 0.0;


	
	for(int i=0; i < numberOfParticles; i++){
	
		gloost::Vector3 normalizedDirection = direction;
		normalizedDirection.normalize();

		normalizedDirection[0] += gloost::getRandomMinMax(-spread,spread);
		normalizedDirection[1] += gloost::getRandomMinMax(-spread,spread);
		normalizedDirection[2] += gloost::getRandomMinMax(-spread,spread);
		random = gloost::getRandomMinMax<float>(0.0,particleSpeed);

		particles[i].x += normalizedDirection[0]*random;

		particles[i].y += normalizedDirection[1]*random;

		particles[i].z += normalizedDirection[2]*random;

		//random = gloost::getRandomMinMax(0.0,2.0);
		particles[i].lifetime += gloost::getRandomMinMax(0.0,0.1);

		particles[i].r = 1.0;
		particles[i].g = gloost::getRandomMinMax(0.6,1.0);
		particles[i].b = gloost::getRandomMinMax(0.0,0.19);
		particles[i].a = 1.0-(particles[i].lifetime/lifetime);

		// reset particles
		if(particles[i].lifetime > lifetime){
		
			particles[i].x =0.0;
			particles[i].y =0.0;
			particles[i].z =0.0;
			particles[i].lifetime = 0.0;
			particles[i].r =0.9;
			particles[i].g =0.0;
			particles[i].b =0.0;
			particles[i].a =0.90;
		}


	}


}

void Particles::setLifetime(float time){
	lifetime = time;
}

void Particles::setSpeed(float speed){
	particleSpeed = speed;
}

std::vector<Particle>& Particles::getParticles(){

	return particles;
}


 Particles::Particles():
			particles(0), lifetime(0.50), spread(0.3), particleSpeed(6.0)
{

	
	
}


Particles::~Particles(void)
{

}
