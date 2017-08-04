<!-- TWO STEPS TO INSTALL POPUP WINDOW:

   1.  Paste the first into the HEAD of your HTML document
   2.  Use the code to open the popup page on your site  -->

<!-- STEP ONE: Copy this code into the HEAD of your HTML document  -->

<HEAD>

<SCRIPT LANGUAGE="JavaScript">
<!-- Idea by:  Nic Wolfe (Nic@TimelapseProductions.com) -->
<!-- Web URL:  http://fineline.xs.mw -->

<!-- This script and many more are available free online at -->
<!-- The JavaScript Source!! http://javascript.internet.com -->

<!-- Begin
function popUp(URL) {
day = new Date();
id = day.getTime();
eval("page" + id + " = window.open(URL, '" + id + "', 'toolbar=0,scrollbars=1,location=0,statusbar=0,menubar=0,resizable=1,width=300,height=800,left = 650,top = 200');");
}
// End -->
</script>


<!-- STEP TWO: Use the following link to open the new window -->

<A HREF="javascript:popUp('/test.html')">Open the Popup Window</A>

<!-- Script Size:  0.73 KB  -->
