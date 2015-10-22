Cinder-Touch 
Cinder 0.9.0 (libcinder.org)
x32, x64
============
Block for receiving and handling touches for 2d Cinder applications. Single or multitouch. Does not handle rotation or include gestures. 

##TO USE :
<ol>
<li>Clone into cinder blocks folder</li>
<li>Using Tinderbox, create a new project and include the Touch block. This will automatically include TUIO as well.</li>
</ol>

##Instructions to use base touch object
Review the classes for more detail, but as a quick reference:
<ol>
<li>In setup, create each Path2d based on a (0,0) coordinate.</li>
<li>Move the Path to the desired starting location on screen by setting mPosition.</li>
<li>If you are translating the space, you have to update mTranslationPos by calling setTranslationPos in the draw function.</li>

Many elements were referenced from the forums/samples related to Cinder, thank you! 