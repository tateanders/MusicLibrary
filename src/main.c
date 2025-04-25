#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "raylib.h"
#include "raymath.h"
#include "clay_renderer_raylib.c"
#include <stdlib.h>
#include <stdio.h>

//define the font names
const int GOTHIC = 0;
const int JAKARTA = 1;
const int PLAYFAIR = 2;

//colors
const Clay_Color DARKMODE = (Clay_Color) {36, 36, 36, 255};
const Clay_Color GARNET = (Clay_Color) {121, 36, 47, 255};
const Clay_Color BLUEGRAY = (Clay_Color) {43, 41, 51, 255};
const Clay_Color OLDGOLD = (Clay_Color) {170, 151, 103, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s", errorData.errorText.chars);
    /*switch(errorData.errorType) {
        // etc
    }*/
}

int main(){
    //make window resizable
    int ScreenWidth = 1128;//GetScreenWidth();
    int ScreenHeight = 705;//GetScreenHeight();
    char* ScreenTitle = "Music Manager";
    Clay_Raylib_Initialize(ScreenWidth, ScreenHeight, ScreenTitle, FLAG_WINDOW_RESIZABLE);

    //allocate the memory and pass it in to a clay arena
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena){
        .memory = malloc(clayRequiredMemory),
        .capacity = clayRequiredMemory
    };
    //initialize the screen
    Clay_Initialize(clayMemory, (Clay_Dimensions) {
        .width = GetScreenWidth(),
        .height = GetScreenHeight(),
    }, (Clay_ErrorHandler) { HandleClayErrors });

    //set up font arrays
    Font fonts[3];
    fonts[GOTHIC] = LoadFontEx("resources/SpecialGothicExpandedOne-Regular.ttf", 48, 0, 400);
    fonts[JAKARTA] = LoadFontEx("resources/PlusJakartaSans-VariableFont_wght.ttf", 48, 0, 400);
    fonts[PLAYFAIR] = LoadFontEx("resources/PlayfairDisplay-VariableFont_wght.ttf", 48, 0, 400);
    //render text
    SetTextureFilter(fonts[GOTHIC].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    while (!WindowShouldClose()){
        //run once per frame
        //make the window resizeable
        Clay_SetLayoutDimensions((Clay_Dimensions){
            .width = GetScreenWidth(),
            .height = GetScreenHeight(),
        });

        Clay_BeginLayout();

        //start build ui
        CLAY({
            .id = CLAY_ID("OuterContainer"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,  // Stack Header on top of the main row
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                //.padding = CLAY_PADDING_ALL(16),
                .childGap = 10
            },
            .backgroundColor = DARKMODE
        }){
        
            // HEADER
            CLAY({
                .id = CLAY_ID("Header"),
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(100) },  // full width, fixed height
                    .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
                    //.padding = CLAY_PADDING_ALL(12)
                },
                .backgroundColor = GARNET,  // dark grey
            }){
                CLAY_TEXT(CLAY_STRING("Music Library"), CLAY_TEXT_CONFIG({
                    .fontId = GOTHIC,
                    .fontSize = 65,
                    .textColor = BLUEGRAY
                }));
            }
        
            // MAIN ROW: contains sidebar and main content
            CLAY({
                .id = CLAY_ID("MainRow"),
                .layout = {
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                    .childGap = 10
                },
                //.backgroundColor = { 0, 0, 255, 255 }
            }){
        
                // SIDEBAR
                CLAY({
                    .id = CLAY_ID("SideBar"),
                    .layout = {
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .sizing = { .width = CLAY_SIZING_FIXED(225), .height = CLAY_SIZING_GROW(0) },
                        .padding = CLAY_PADDING_ALL(10),
                        .childGap = 10
                    },
                    .backgroundColor = OLDGOLD
                    //.cornerRadius = 8
                }){}
        
                // MAIN CONTENT (empty for now)
                CLAY({
                    .id = CLAY_ID("MainContent"),
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(1), CLAY_SIZING_GROW(0) },
                        .padding = CLAY_PADDING_ALL(10)
                    },
                    .backgroundColor = BLUEGRAY  // even darker bg
                }){}
        
            }
        }
        //end build ui

        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        //now pass everything to raylib
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();
    }
}