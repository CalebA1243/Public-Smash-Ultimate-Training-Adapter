# Public Smash Ultimate Training Adapter

This is currently a work in progress… 

# Usage
There are four DI modes and four escape modes. The DI modes are cycled by pressing left on the D-Pad and the escape modes are cycled by pressing right on the D-Pad. Pressing start resets the controller.

*Note- Every frame means close to every frame. This should improve in a later version.*

DI Modes:
1.  Alternating left/right DI- The controller will change directions every frame.
2.  Random DI-Every frame, the controller will chose a new direction to DI in.
3.  Set DI- When choosing this mode, hold a direction, and the controller will continue to DI in that direction.
4. Set SDI- When choosing this mode, hold a direction, and the controller will continue to SDI frame perfectly in that direction.

Escape Modes: 
1. Trigger/Air-dodge/Shield- Controller will spam the trigger button, which has the character mash air dodge to get out of combos.
2. Jump - The controller will mash jump. Useful if mode is chosen after switching opponent to smash in training, grabbing the opponent, switching to jump while their still stuck in grab, then unpausing, and testing a throw followup. This method guarantees the opponent to have a double jump after the throw
3. Special- When choosing this mode, hold and direction (or no direction at all) and the cpu will mash the special in the direction you choose. Ex: Hold up to get Marth to mash up b or down to get Pokemon Trainer to switch
4. Aerial- Similar to option three, hold a direction down on the analogue stick as you enter this mode, and the controller will mash an aerial/smash attack/jab in the direction you choose. Ex: Back to get inkling to spam back air, no direction to get Luigi to spam nair.


Currently, I’m in the process of testing out different methods for input recording. This commit is just to help others see how the project is structured and to allow those with differently soldered pins utilize basic aspects of the mod.