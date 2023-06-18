matlang2c:	assignmentParsing.o declarationEvaluation.o exprParsing.o forParsing.o funcNames.h globalVars.o header.h main.o statementParsing.o stringManipulationFns.o testFuncs.o
	gcc assignmentParsing.o declarationEvaluation.o exprParsing.o forParsing.o globalVars.o main.o statementParsing.o stringManipulationFns.o testFuncs.o -o matlang2c

assignmentParsing.o: assignmentParsing.c header.h
	@echooff
	gcc -c assignmentParsing.c

declarationEvaluation.o: declarationEvaluation.c header.h
	@echooff
	gcc -c declarationEvaluation.c

exprParsing.o: exprParsing.c header.h
	gcc -c exprParsing.c

forParsing.o: forParsing.c header.h
	gcc -c forParsing.c

main.o: main.c header.h
	gcc -c main.c

statementParsing.o: statementParsing.c header.h
	gcc -c statementParsing.c

globalVars.o: globalVars.c header.h
	gcc -c globalVars.c

stringManipulationFns.o: stringManipulationFns.c header.h
	gcc -c stringManipulationFns.c

testFuncs.o: testFuncs.c header.h
	gcc -c testFuncs.c
