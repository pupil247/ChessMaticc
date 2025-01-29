#include "static.h"
#include "math.h"
#include <algorithm>
#include "esp_log.h"

Static::Static(int pixelCount, int refreshSpeed) : 
	Effect(pixelCount, refreshSpeed)
{
	
	partage = Partage::getInstance();
	indexLigne = 0;
	indexColonne = 0;
	cnt = 0;
	onOff = 0;
}

void Static::Run(Pixels* pixels)
{
	//cnt++;
	//if(cnt == ){
		//cnt = 0;
		onOff = !onOff;
		//ESP_LOGI("led","toggle led");
	//}	
	reedBoard = partage->getReedBoard();
	chessBoard = partage->getChessBoard();
	for (int i = 0; i < pixelCount; i++)
	{
		

		Pixel pixel = pixels->GetPixel(i);
		/*if (reedBoard[indexColonne].getStatus()[indexLigne] == true && indexColonne < 8){

			pixel.red = 0;
			pixel.green = 0;
			pixel.blue = 255;
			pixel.white = 100;
			//if(chessBoard[indexColonne][indexLigne].getType() != ChessPieceType::EMPTY){
				//ESP_LOGI("TEST", "step 3");	
			//}
		}
		else
		{
			pixel.red = 0;
			pixel.green = 0;
			pixel.blue = 0;
			pixel.white = 0;	
		}*/
		if(partage->getPieceToCapture().x == indexColonne && partage->getPieceToCapture().y == indexLigne){
			if(onOff){
				pixel.red = 255;
				pixel.green = 255;
				pixel.blue = 0;
				pixel.white = 0;
			}
			else{
				pixel.red = 0;
				pixel.green = 0;
				pixel.blue = 0;
				pixel.white = 0;	
			}
				
		}
		else if(partage->isLegalPosition(indexColonne,indexLigne)){
			pixel.red = 0;
			pixel.green = 255;
			pixel.blue = 0;
			pixel.white = 0;
		}
		else if(partage->getPieceToMove().x == indexColonne && partage->getPieceToMove().y == indexLigne){
			pixel.red = 255;
			pixel.green = 0;
			pixel.blue = 255;
			pixel.white = 0;	
		}
		else if(partage->getPieceToPromote().x == indexColonne && partage->getPieceToPromote().y == indexLigne){
			
			if(onOff){
				pixel.red = 120;
				pixel.green = 40;
				pixel.blue = 0;
				pixel.white = 50;
			}
			else{
				pixel.red = 0;
				pixel.green = 0;
				pixel.blue = 0;
				pixel.white = 0;	
			}		
		}
		
		
		else if (reedBoard[indexColonne].getStatus()[indexLigne] == true && chessBoard[indexColonne][indexLigne].getType() != ChessPieceType::EMPTY) //reedSwitch == 1 && chessboard[i] is not empty
        {//Si pièce est présente et à bonne place on met bleu	
			pixel.red = 0;
			pixel.green = 0;
			pixel.blue = 255;
			pixel.white = 0;
		}
        else if(partage->isNonValid(indexColonne,indexLigne))//reedSwitch == 0 && chessboard[i] is not empty Si pièce est pas presente, mais qu'elle devrait, on met rouge
        {
			ESP_LOGI("flag","nonvalid ");
            pixel.red = 255;
			pixel.green = 0;
			pixel.blue = 0;
			pixel.white = 0;
        }
        
        else //sinon c<est que case vide
        {
            pixel.red = 0;
			pixel.green = 0;
			pixel.blue = 0;
			pixel.white = 0;       
        }

		pixels->SetPixel(i, pixel);
		indexLigne++;
		if(indexLigne > 7){
			indexColonne++;
			indexLigne = 0;	
		}
			
		if(indexColonne > 7)
			indexColonne = 0;
	}
	
	WriteAndWait(pixels);
}