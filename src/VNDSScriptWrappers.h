#ifndef VNDSSCRIPTWRAPPERS
#define VNDSSCRIPTWRAPPERS

#define _VNDSWAITDISABLE 1

char nextVndsBustshotSlot=0;

void vndswrapper_text(nathanscriptVariable* _passedArguments, int _numArguments, nathanscriptVariable** _returnedReturnArray, int* _returnArraySize){
	if (nathanvariableToString(&_passedArguments[0])[0]=='@'){
		OutputLine(&(nathanvariableToString(&_passedArguments[0])[1]),Line_ContinueAfterTyping,isSkipping);
		outputLineWait();
	}else if (nathanvariableToString(&_passedArguments[0])[0]=='!'){ // Blank line that requires button push
		OutputLine("\n",Line_WaitForInput,isSkipping);
		outputLineWait();
		currentLine--; // Later in this function we add 1 to this variable, so it evens out.
	}else if (!(nathanvariableToString(&_passedArguments[0])[0]=='~')){ // If we're not making a blank line that doesn't require button push
		OutputLine(nathanvariableToString(&_passedArguments[0]),Line_WaitForInput,isSkipping);
		outputLineWait();
	}
	currentLine++;
	if (currentLine==MAXLINES){
		// TODO - Just shift everything else up
		ClearMessageArray();
		currentLine=0;
	}
}
void vndswrapper_choice(nathanscriptVariable* _passedArguments, int _numArguments, nathanscriptVariable** _returnedReturnArray, int* _returnArraySize){
	char* _choiceSet = nathanvariableToString(&_passedArguments[0]);
	//|
	int i;
	short _totalChoices=1;
	for (i=0;i<strlen(_choiceSet);i++){
		if (_choiceSet[i]=='|'){
			_totalChoices++;
		}
	}

	// New array
	nathanscriptVariable* _fakeArgumentArray = malloc(sizeof(nathanscriptVariable)*2);

	_fakeArgumentArray[0].variableType = NATHAN_TYPE_FLOAT;
	_fakeArgumentArray[0].value = malloc(sizeof(float));
	POINTER_TOFLOAT(_fakeArgumentArray[0].value)=_totalChoices;

	char** _argumentTable = malloc(sizeof(char*)*(_totalChoices+1));
	_fakeArgumentArray[1].variableType = NATHAN_TYPE_ARRAY;
	_fakeArgumentArray[1].value = _argumentTable;

	memcpy(&(_argumentTable[0]),&(_totalChoices),sizeof(short));
	int _currentMainStringIndex=0;
	for (i=0;i<_totalChoices;i++){
		int _thisSegmentStartIndex=_currentMainStringIndex;
		for (;;_currentMainStringIndex++){
			if (_choiceSet[_currentMainStringIndex]=='|' || _choiceSet[_currentMainStringIndex]==0){
				_choiceSet[_currentMainStringIndex]=0;
				char* _newBuffer = malloc(strlen(&(_choiceSet[_thisSegmentStartIndex]))+1);
				if (_choiceSet[_thisSegmentStartIndex]==' '){ // Trim up to one leading space
					_thisSegmentStartIndex++;
				}
				strcpy(_newBuffer,&(_choiceSet[_thisSegmentStartIndex]));
				_argumentTable[i+1]=_newBuffer;
				_currentMainStringIndex++;
				break;
			}
		}
	}

	scriptSelect(_fakeArgumentArray,2,NULL,NULL);
	char _numberToStringBuffer[20];
	sprintf(_numberToStringBuffer,"%d",lastSelectionAnswer+1);
	genericSetVar("selected","=",_numberToStringBuffer,&nathanscriptGamevarList,&nathanscriptTotalGamevar);
}
// jump file.scr [label]
void vndswrapper_jump(nathanscriptVariable* _passedArguments, int _numArguments, nathanscriptVariable** _returnedReturnArray, int* _returnArraySize){
	
	char _tempstringconcat[strlen(scriptFolder)+strlen(nathanvariableToString(&_passedArguments[0]))+1];
	strcpy(_tempstringconcat,scriptFolder);
	strcat(_tempstringconcat,nathanvariableToString(&_passedArguments[0]));
	if (checkFileExist(_tempstringconcat)==0){
		LazyMessage("File not found,",_tempstringconcat,NULL,NULL);
		return;
	}
	fclose(nathanscriptCurrentOpenFile);
	nathanscriptCurrentOpenFile = fopen(_tempstringconcat,"r");
	nathanscriptCurrentLine=1;
	if (_numArguments==2){ // Optional label argument
		genericGotoLabel(nathanvariableToString(&_passedArguments[1]));
	}
}
void vndswrapper_delay(nathanscriptVariable* _passedArguments, int _numArguments, nathanscriptVariable** _returnedReturnArray, int* _returnArraySize){
	#if !_VNDSWAITDISABLE
	long _totalMillisecondWaitTime = ((nathanvariableToFloat(&_passedArguments[0])/(float)60)*1000);
	wait(_totalMillisecondWaitTime);
	#else
	printf("delay command is disable.\n");
	#endif
}
void vndswrapper_cleartext(nathanscriptVariable* _passedArguments, int _numArguments, nathanscriptVariable** _returnedReturnArray, int* _returnArraySize){
	ClearMessageArray();
}
// bgload filename.extention [dsFadeinFrames]
void vndswrapper_bgload(nathanscriptVariable* _passedArguments, int _numArguments, nathanscriptVariable** _returnedReturnArray, int* _returnArraySize){
	DrawScene(nathanvariableToString(&_passedArguments[0]),_numArguments==2 ? floor((nathanvariableToFloat(&_passedArguments[1])/60)*1000) : 0);
	nextVndsBustshotSlot=0;
}
// setimg file x y
// setimg MGE_000099.png 75 0
void vndswrapper_setimg(nathanscriptVariable* _passedArguments, int _numArguments, nathanscriptVariable** _returnedReturnArray, int* _returnArraySize){
	//void DrawBustshot(unsigned char passedSlot, char* _filename, int _xoffset, int _yoffset, int _layer, int _fadeintime, int _waitforfadein, int _isinvisible){
	DrawBustshot(nextVndsBustshotSlot++, nathanvariableToString(&_passedArguments[0]), nathanvariableToInt(&_passedArguments[1]), nathanvariableToInt(&_passedArguments[2]), 0, 0, 0, 0);
}

void vndswrapper_random(nathanscriptVariable* _passedArguments, int _numArguments, nathanscriptVariable** _returnedReturnArray, int* _returnArraySize){
	
}

#endif