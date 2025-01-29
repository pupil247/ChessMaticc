/**
 * @file static.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief only custom effect for the ledboard, reflect the chessboard state on the ledboard when playing a game
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "effect.h"
#include "partage.h"

class Partage;

/**
 * @brief class for custom ledboard effect(effect used during games)
 * 
 */
class Static : Effect
{
public:
	/**
	 * @brief Construct a new Static effect object
	 * 
	 * @param pixelCount 
	 * @param refreshSpeed 
	 */
	Static(int pixelCount, int refreshSpeed);
	/**
	 * @brief run the effect on the ledboard
	 * 
	 * @param pixels 
	 */
	void Run(Pixels* pixels);
	
private:
	std::vector<ReedLine> reedBoard;
	Partage * partage;
	std::vector<std::vector<ChessPiece>> chessBoard;
	uint8_t indexLigne;
	uint8_t indexColonne;
	uint8_t cnt;
	bool onOff;
};
