/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2014, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <gtest/gtest.h>
#include <esvg/esvg.h>
#include "main.h"

TEST(TestCap, butt) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,75 80,75' stroke='green' stroke-width='20' fill='none' stroke-linecap='butt'/>"
	                 "	<polyline points='80,25 20,25' stroke='orange' stroke-width='20' fill='none' stroke-linecap='butt'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_butt.svg", data);
	doc.generateAnImage("TestCap_butt.bmp", g_visualDebug);
}

TEST(TestCap, round) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,75 80,75' stroke='green' stroke-width='20' fill='none' stroke-linecap='round'/>"
	                 "	<polyline points='80,25 20,25' stroke='orange' stroke-width='20' fill='none' stroke-linecap='round'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_round.svg", data);
	doc.generateAnImage("TestCap_round.bmp", g_visualDebug);
}

TEST(TestCap, square) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,75 80,75' stroke='green' stroke-width='20' fill='none' stroke-linecap='square'/>"
	                 "	<polyline points='80,25 20,25' stroke='orange' stroke-width='20' fill='none' stroke-linecap='square'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_square.svg", data);
	doc.generateAnImage("TestCap_square.bmp", g_visualDebug);
}


TEST(TestCap, buttVert) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='25,20 25,80' stroke='green' stroke-width='20' fill='none' stroke-linecap='butt'/>"
	                 "	<polyline points='75,80 75,20' stroke='orange' stroke-width='20' fill='none' stroke-linecap='butt'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_buttVert.svg", data);
	doc.generateAnImage("TestCap_buttVert.bmp", g_visualDebug);
}

TEST(TestCap, roundVert) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='25,20 25,80' stroke='green' stroke-width='20' fill='none' stroke-linecap='round'/>"
	                 "	<polyline points='75,80 75,20' stroke='orange' stroke-width='20' fill='none' stroke-linecap='round'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_roundVert.svg", data);
	doc.generateAnImage("TestCap_roundVert.bmp", g_visualDebug);
}

TEST(TestCap, squareVert) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='25,20 25,80' stroke='green' stroke-width='20' fill='none' stroke-linecap='square'/>"
	                 "	<polyline points='75,80 75,20' stroke='orange' stroke-width='20' fill='none' stroke-linecap='square'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_squareVert.svg", data);
	doc.generateAnImage("TestCap_squareVert.bmp", g_visualDebug);
}



TEST(TestCap, buttDiag1) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,20 80,80' stroke='green' stroke-width='20' fill='none' stroke-linecap='butt'/>"
	                 "	<polyline points='80,20 20,80' stroke='orange' stroke-width='20' fill='none' stroke-linecap='butt'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_buttDiag1.svg", data);
	doc.generateAnImage("TestCap_buttDiag1.bmp", g_visualDebug);
}

TEST(TestCap, roundDiag1) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,20 80,80' stroke='green' stroke-width='20' fill='none' stroke-linecap='round'/>"
	                 "	<polyline points='80,20 20,80' stroke='orange' stroke-width='20' fill='none' stroke-linecap='round'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_roundDiag1.svg", data);
	doc.generateAnImage("TestCap_roundDiag1.bmp", g_visualDebug);
}

TEST(TestCap, squareDiag1) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,20 80,80' stroke='green' stroke-width='20' fill='none' stroke-linecap='square'/>"
	                 "	<polyline points='80,20 20,80' stroke='orange' stroke-width='20' fill='none' stroke-linecap='square'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_squareDiag1.svg", data);
	doc.generateAnImage("TestCap_squareDiag1.bmp", g_visualDebug);
}


TEST(TestCap, buttDiag2) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,80 80,20' stroke='green' stroke-width='20' fill='none' stroke-linecap='butt'/>"
	                 "	<polyline points='80,80 20,20' stroke='orange' stroke-width='20' fill='none' stroke-linecap='butt'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_buttDiag2.svg", data);
	doc.generateAnImage("TestCap_buttDiag2.bmp", g_visualDebug);
}

TEST(TestCap, roundDiag2) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,80 80,20' stroke='green' stroke-width='20' fill='none' stroke-linecap='round'/>"
	                 "	<polyline points='80,80 20,20' stroke='orange' stroke-width='20' fill='none' stroke-linecap='round'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_roundDiag2.svg", data);
	doc.generateAnImage("TestCap_roundDiag2.bmp", g_visualDebug);
}

TEST(TestCap, squareDiag2) {
	std::string data("<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
	                 "<svg height='100' width='100'>"
	                 "	<polyline points='20,80 80,20' stroke='green' stroke-width='20' fill='none' stroke-linecap='square'/>"
	                 "	<polyline points='80,80 20,20' stroke='orange' stroke-width='20' fill='none' stroke-linecap='square'/>"
	                 "</svg>");
	esvg::Document doc;
	doc.parse(data);
	etk::FSNodeWriteAllData("TestCap_squareDiag2.svg", data);
	doc.generateAnImage("TestCap_squareDiag2.bmp", g_visualDebug);
}

