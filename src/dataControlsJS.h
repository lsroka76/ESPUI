const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_RELOAD=201;const UPDATE_OFFSET=100;const UI_EXTEND_GUI=210;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPDATE_GAUGE=117;const UI_ACCEL=18;const UPDATE_ACCEL=118;const UI_SEPARATOR=19;const UPDATE_SEPARATOR=119;const UI_TIME=20;const UPDATE_TIME=120;const UI_FILEDISPLAY=21;const UPDATE_FILEDISPLAY=121;const UI_FRAGMENT=98;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var controlAssemblyArray=new Object();var FragmentAssemblyTimer=new Array();var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_DARK:case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;var WebSocketTimer=null;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);let idData=savedData[id];return Array.isArray(idData)?idData:[];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");conStatusError();start();}
function conStatusError(){FragmentAssemblyTimer.forEach(element=>{clearInterval(element);});FragmentAssemblyTimer=new Array();controlAssemblyArray=new Array();if(true===websockConnected){websockConnected=false;websock.close();$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart,});}}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){let location=window.location.hostname;let port=window.location.port;document.addEventListener("visibilitychange",handleVisibilityChange,false);if(port!=""||port!=80||port!=443){websock=new WebSocket("ws://"+location+":"+port+"/ws");}else{websock=new WebSocket("ws://"+location+"/ws");}
if(null===WebSocketTimer){WebSocketTimer=setInterval(function(){if(websock.readyState===3){restart();}},5000);}
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;FragmentAssemblyTimer.forEach(element=>{clearInterval(element);});FragmentAssemblyTimer=new Array();controlAssemblyArray=new Array();};websock.onclose=function(evt){console.log("websock close");conStatusError();FragmentAssemblyTimer.forEach(element=>{clearInterval(element);});FragmentAssemblyTimer=new Array();controlAssemblyArray=new Array();};websock.onerror=function(evt){console.log("websock Error");restart();FragmentAssemblyTimer.forEach(element=>{clearInterval(element);});FragmentAssemblyTimer=new Array();controlAssemblyArray=new Array();};var handleEvent=function(evt){try{var data=JSON.parse(evt.data);}
catch(Event){console.error(Event);websock.send("uiok:"+0);return;}
var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>(data.controls.length-1)){websock.send("uiok:"+(data.controls.length-1));}
break;case UI_EXTEND_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>data.startindex+(data.controls.length-1)){websock.send("uiok:"+(data.startindex+(data.controls.length-1)));}
break;case UI_RELOAD:window.location.reload();break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GAUGE:case UI_SEPARATOR:if(data.visible)addToHTML(data);break;case UI_BUTTON:if(data.visible){addToHTML(data);$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();$(this).addClass("pressed");buttonclick(data.id,true);},touchend:function(e){e.preventDefault();$(this).removeClass("pressed");buttonclick(data.id,false);},});}
break;case UI_SWITCHER:if(data.visible){addToHTML(data);switcher(data.id,data.value);}
break;case UI_CPAD:case UI_PAD:if(data.visible){addToHTML(data);$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);},});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);},});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);},});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);},});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);},});}
break;case UI_SLIDER:if(data.visible){addToHTML(data);rangeSlider(!sliderContinuous);}
break;case UI_TAB:if(data.visible){$("#tabsnav").append("<li><a onmouseup='tabclick("+data.id+")' href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
e.preventDefault();});}
break;case UI_OPTION:if(data.parentControl){var parent=$("#select"+data.parentControl);if(data.value=="-3")
break;if(data.value=="-2")
parent.append("<option id='option"+
data.id+
"' value='"+
data.value+
"' "+"hidden>"+
data.label+
"</option>");else
parent.append("<option id='option"+
data.id+
"' value='"+
data.value+
"' "+
data.selected+
">"+
data.label+
"</option>");}
if(data.hasOwnProperty('secondParent')){var parentB=$("#select"+data.secondParent);if(data.value=="-2")
parentB.append("<option id='optionB"+
data.id+
"' value='"+
data.value+
"' "+"hidden>"+
data.label+
"</option>");else
parentB.append("<option id='optionB"+
data.id+
"' value='"+
data.value+
"' "+
data.selected+
">"+
data.label+
"</option>");}
if(data.hasOwnProperty('thirdParent')){var parentC=$("#select"+data.thirdParent);if(data.value=="-2")
parentC.append("<option id='optionC"+
data.id+
"' value='"+
data.value+
"' "+"hidden>"+
data.label+
"</option>");else
parentC.append("<option id='optionC"+
data.id+
"' value='"+
data.value+
"' "+
data.selected+
">"+
data.label+
"</option>");}
break;case UI_MIN:if(data.parentControl){if($('#sl'+data.parentControl).length){$('#sl'+data.parentControl).attr("min",data.value);}else if($('#num'+data.parentControl).length){$('#num'+data.parentControl).attr("min",data.value);}}
break;case UI_MAX:if(data.parentControl){if($('#sl'+data.parentControl).length){$('#sl'+data.parentControl).attr("max",data.value);}else if($('#text'+data.parentControl).length){$('#text'+data.parentControl).attr("maxlength",data.value);}else if($('#num'+data.parentControl).length){$('#num'+data.parentControl).attr("max",data.value);}}
break;case UI_STEP:if(data.parentControl){if($('#sl'+data.parentControl).length){$('#sl'+data.parentControl).attr("step",data.value);}else if($('#num'+data.parentControl).length){$('#num'+data.parentControl).attr("step",data.value);}}
break;case UI_GRAPH:if(data.visible){addToHTML(data);graphData[data.id]=restoreGraphData(data.id);renderGraphSvg(graphData[data.id],"graph"+data.id);}
break;case ADD_GRAPH_POINT:var ts=new Date().getTime();graphData[data.id].push({x:ts,y:data.value});saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_ACCEL:if(hasAccel)break;hasAccel=true;if(data.visible){addToHTML(data);requestOrientationPermission();}
break;case UI_FILEDISPLAY:if(data.visible)
{addToHTML(data);FileDisplayUploadFile(data);}
break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);if(data.hasOwnProperty('elementStyle')){$("#l"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SLIDER:$("#sl"+data.id).attr("value",data.value)
slider_move($("#sl"+data.id).parent().parent(),data.value,"100",false);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#num"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#text"+data.id).attr("style",data.elementStyle);}
if(data.hasOwnProperty('inputType')){$("#text"+data.id).attr("type",data.inputType);}
break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#select"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_OPTION:var parent=$("#select"+data.parentControl);var option=$("#option"+data.id);if(data.hasOwnProperty('secondParent')){var parentB=$("#select"+data.secondParent);var optionB=$("#optionB"+data.id);if(optionB.length>0){if(data.value=="-2")
optionB.remove();else{optionB.val(data.value);optionB.text(data.label);}}
else{if(Number(data.value)>-2)
parentB.append("<option id='optionB"+
data.id+
"' value='"+
data.value+
"' "+">"+
data.label+
"</option>");}}
if(data.hasOwnProperty('thirdParent')){var parentC=$("#select"+data.thirdParent);var optionC=$("#optionC"+data.id);if(optionC.length>0){if(data.value=="-2")
optionC.remove();else{optionC.val(data.value);optionC.text(data.label);}}
else{if(Number(data.value)>-2)
parentC.append("<option id='optionC"+
data.id+
"' value='"+
data.value+
"' "+">"+
data.label+
"</option>");}}
if(option.length>0){if(data.value=="-2")
option.remove();else{option.val(data.value);option.text(data.label);}}
else{if(Number(data.value)>-2)
parent.append("<option id='option"+
data.id+
"' value='"+
data.value+
"' "+">"+
data.label+
"</option>");}
break;case UPDATE_BUTTON:$("#btn"+data.id).val(data.value);$("#btn"+data.id).text(data.value);if(data.hasOwnProperty('elementStyle')){$("#btn"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#gauge"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_ACCEL:break;case UPDATE_TIME:var rv=new Date().toISOString();websock.send("time:"+rv+":"+data.id);break;case UPDATE_FILEDISPLAY:FileDisplayUploadFile(data);break;case UI_FRAGMENT:let FragmentLen=data.length;let FragementOffset=data.offset;let NextFragmentOffset=FragementOffset+FragmentLen;let Total=data.total;let Arrived=(FragmentLen+FragementOffset);let FragmentFinal=Total===Arrived;if(!data.hasOwnProperty('control'))
{console.error("UI_FRAGMENT:Missing control record, skipping control");break;}
let control=data.control;StopFragmentAssemblyTimer(data.control.id);if(0===FragementOffset)
{controlAssemblyArray[control.id]=data;controlAssemblyArray[control.id].offset=NextFragmentOffset;StartFragmentAssemblyTimer(control.id);let TotalRequest=JSON.stringify({'id':control.id,'offset':NextFragmentOffset});websock.send("uifragmentok:"+0+": "+TotalRequest+":");break;}
if("undefined"===typeof controlAssemblyArray[control.id])
{console.error("Missing first fragment for control: "+control.id);StartFragmentAssemblyTimer(control.id);let TotalRequest=JSON.stringify({'id':control.id,'offset':0});websock.send("uifragmentok:"+0+": "+TotalRequest+":");break;}
if(FragementOffset!==controlAssemblyArray[control.id].offset)
{console.error("Wrong next fragment. Expected: "+controlAssemblyArray[control.id].offset+" Got: "+FragementOffset);StartFragmentAssemblyTimer(control.id);let TotalRequest=JSON.stringify({'id':control.id,'offset':controlAssemblyArray[control.id].length+controlAssemblyArray[control.id].offset});websock.send("uifragmentok:"+0+": "+TotalRequest+":");break;}
controlAssemblyArray[control.id].control.value+=control.value;controlAssemblyArray[control.id].offset=NextFragmentOffset;if(true===FragmentFinal)
{var fauxEvent={data:JSON.stringify(controlAssemblyArray[control.id].control),};handleEvent(fauxEvent);controlAssemblyArray[control.id]=null;}
else
{StartFragmentAssemblyTimer(control.id);let TotalRequest=JSON.stringify({'id':control.id,'offset':NextFragmentOffset});websock.send("uifragmentok:"+0+": "+TotalRequest+":");}
break;default:console.error("Unknown type or event");console.error(data.type);break;}
if(data.type>=UI_TITEL&&data.type<UPDATE_OFFSET){processEnabled(data);}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.hasOwnProperty('panelStyle')){$("#id"+data.id).attr("style",data.panelStyle);}
if(data.hasOwnProperty('visible')){if(data['visible'])
$("#id"+data.id).show();else
$("#id"+data.id).hide();}
if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}
processEnabled(data);}
$(".range-slider__range").each(function(){$(this)[0].value=$(this).attr("value");$(this).next().html($(this).attr("value"));});};websock.onmessage=handleEvent;}
async function FileDisplayUploadFile(data)
{let text=await downloadFile(data.value);let ItemToUpdateId="fd"+data.id;$("#"+ItemToUpdateId).val(text);$("#"+ItemToUpdateId).css("textAlign","left");$("#"+ItemToUpdateId).css("white-space","nowrap");$("#"+ItemToUpdateId).css("overflow","scroll");$("#"+ItemToUpdateId).css("overflow-y","scroll");$("#"+ItemToUpdateId).css("overflow-x","scroll");$("#"+ItemToUpdateId).scrollTop($("#"+ItemToUpdateId).val().length);}
async function downloadFile(filename)
{let response=await fetch(filename);if(response.status!=200){throw new Error("File Read Server Error: '"+response.status+"'");}
let text_data=await response.text();return text_data;}
function StartFragmentAssemblyTimer(Id)
{StopFragmentAssemblyTimer(Id);FragmentAssemblyTimer[Id]=setInterval(function(_Id)
{if("undefined"!==typeof controlAssemblyArray[_Id])
{if(null!==controlAssemblyArray[_Id])
{let TotalRequest=JSON.stringify({'id':controlAssemblyArray[_Id].control.id,'offset':controlAssemblyArray[_Id].offset});websock.send("uifragmentok:"+0+": "+TotalRequest+":");}}},1000,Id);}
function StopFragmentAssemblyTimer(Id)
{if("undefined"!==typeof FragmentAssemblyTimer[Id])
{if(FragmentAssemblyTimer[Id])
{clearInterval(FragmentAssemblyTimer[Id]);}}}
function sliderchange(number){var val=$("#sl"+number).val();websock.send("slvalue:"+val+":"+number);$(".range-slider__range").each(function(){$(this).attr("value",$(this)[0].value);});}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function tabclick(number){var val=$("#tab"+number).val();websock.send("tabvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if($("#btn"+number).prop("disabled")){return;}
if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){if($("#id"+number+" nav").hasClass("disabled")){return;}
switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if(!$("#sl"+number).hasClass("checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var range=$(".range-slider__range");var slidercb=function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));};range.on({input:function(){$(this).next().html(this.value)}});range.each(function(){$(this).next().html(this.value);if($(this).attr("callbackSet")!="true"){if(!isDiscrete){$(this).on({input:slidercb});}else{$(this).on({change:slidercb});}
$(this).attr("callbackSet","true");}});};var addToHTML=function(data){panelStyle=data.hasOwnProperty('panelStyle')?" style='"+data.panelStyle+"' ":"";panelwide=data.hasOwnProperty('wide')?"wide":"";if(!data.hasOwnProperty('parentControl')||$("#tab"+data.parentControl).length>0){var parent=data.hasOwnProperty('parentControl')?$("#tab"+data.parentControl):$("#row");var html="";switch(data.type){case UI_LABEL:case UI_BUTTON:case UI_SWITCHER:case UI_CPAD:case UI_PAD:case UI_SLIDER:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GRAPH:case UI_GAUGE:case UI_ACCEL:case UI_FILEDISPLAY:html="<div id='id"+data.id+"' "+panelStyle+" class='two columns "+panelwide+" card tcenter "+
colorClass(data.color)+"'><h5>"+data.label+"</h5><hr/>"+
elementHTML(data)+
"</div>";break;case UI_SEPARATOR:html="<div id='id"+data.id+"' "+panelStyle+" class='sectionbreak columns'>"+
"<h5>"+data.label+"</h5><hr/></div>";break;case UI_TIME:break;}
parent.append(html);}else{var parent=$("#id"+data.parentControl);parent.append(elementHTML(data));}}
var elementHTML=function(data){var id=data.id
var elementStyle=data.hasOwnProperty('elementStyle')?" style='"+data.elementStyle+"' ":"";var inputType=data.hasOwnProperty('inputType')?" type='"+data.inputType+"' ":"";switch(data.type){case UI_LABEL:return"<span id='l"+id+"' "+elementStyle+
" class='label label-wrap'>"+data.value+"</span>";case UI_FILEDISPLAY:return"<textarea id='fd"+id+"' rows='4' "+elementStyle+
" class='label label-wrap'>"+"</textarea>";case UI_BUTTON:return"<button id='btn"+id+"' "+elementStyle+
" onmousedown='buttonclick("+id+", true)'"+
" onmouseup='buttonclick("+id+", false)'>"+
data.value+"</button>";case UI_SWITCHER:return"<label id='sl"+id+"' "+elementStyle+
" class='switch "+(data.value=="1"?"checked":"")+
(data.hasOwnProperty('vertical')?" vert-switcher ":"")+
"'>"+
"<div class='in'>"+
"<input type='checkbox' id='s"+id+"' onClick='switcher("+id+",null)' "+
(data.value=="1"?"checked":"")+"/></div></label>";case UI_CPAD:case UI_PAD:return"<nav class='control'><ul>"+
"<li><a onmousedown='padclick(UP, "+id+", true)' "+
"onmouseup='padclick(UP, "+id+", false)' id='pf"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(RIGHT, "+id+", true)' "+
"onmouseup='padclick(RIGHT, "+id+", false)' id='pr"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(LEFT, "+id+", true)' "+
"onmouseup='padclick(LEFT, "+id+", false)' id='pl"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(DOWN, "+id+", true)' "+
"onmouseup='padclick(DOWN, "+id+", false)' id='pb"+id+"'>&#9650;</a></li>"+
"</ul>"+
(data.type==UI_CPAD?"<a class='confirm' onmousedown='padclick(CENTER,"+id+", true)' "+
"onmouseup='padclick(CENTER, "+id+", false)' id='pc"+id+"'>OK</a>":"")+
"</nav>";case UI_SLIDER:return"<div class='range-slider "+
(data.hasOwnProperty('vertical')?" vert-slider ":"")+
"'>"+
"<input id='sl"+id+"' type='range' min='0' max='100' value='"+data.value+"' "+
elementStyle+" class='range-slider__range'><span class='range-slider__value'>"+
data.value+"</span></div>";case UI_NUMBER:return"<input style='color:black; "+data.elementStyle+"' id='num"+id+
"' type='number' value='"+data.value+"' onchange='numberchange("+id+")' />";case UI_TEXT_INPUT:return"<input "+inputType+"style='color:black; "+data.elementStyle+"' id='text"+id+
"' value='"+data.value+"' onchange='textchange("+id+")' />";case UI_SELECT:return"<select style='color:black; "+data.elementStyle+"' id='select"+id+
"' onchange='selectchange("+id+")' />";case UI_GRAPH:return"<figure id='graph"+id+"'><figcaption>"+data.label+"</figcaption></figure>";case UI_GAUGE:return"WILL BE A GAUGE <input style='color:black;' id='gauge"+id+
"' type='number' value='"+data.value+"' onchange='numberchange("+id+")' />";case UI_ACCEL:return"ACCEL // Not implemented fully!<div class='accelerometer' id='accel"+id+
"' ><div class='ball"+id+"'></div><pre class='accelerometeroutput"+id+"'></pre>";default:return"";}}
var processEnabled=function(data){switch(data.type){case UI_SWITCHER:case UPDATE_SWITCHER:if(data.enabled){$("#sl"+data.id).removeClass('disabled');$("#s"+data.id).prop("disabled",false);}else{$("#sl"+data.id).addClass('disabled');$("#s"+data.id).prop("disabled",true);}
break;case UI_SLIDER:case UPDATE_SLIDER:$("#sl"+data.id).prop("disabled",!data.enabled);break;case UI_NUMBER:case UPDATE_NUMBER:$("#num"+data.id).prop("disabled",!data.enabled);break;case UI_TEXT_INPUT:case UPDATE_TEXT_INPUT:$("#text"+data.id).prop("disabled",!data.enabled);break;case UI_SELECT:case UPDATE_SELECT:$("#select"+data.id).prop("disabled",!data.enabled);break;case UI_BUTTON:case UPDATE_BUTTON:$("#btn"+data.id).prop("disabled",!data.enabled);break;case UI_PAD:case UI_CPAD:case UPDATE_PAD:case UPDATE_CPAD:case UI_FILEDISPLAY:case UPDATE_FILEDISPLAY:if(data.enabled){$("#id"+data.id+" nav").removeClass('disabled');}else{$("#id"+data.id+" nav").addClass('disabled');}
break;}}
)=====";

const uint8_t JS_CONTROLS_GZIP[5413] PROGMEM = { 31,139,8,0,137,30,138,105,2,255,213,60,105,119,219,186,142,223,243,43,20,245,158,218,158,56,137,221,237,181,118,148,30,199,113,90,207,203,54,137,243,122,103,122,59,57,178,69,199,154,40,146,158,36,103,25,95,255,247,1,184,137,212,98,59,219,204,157,47,173,69,130,0,8,128,36,8,2,25,5,126,156,24,23,253,203,254,113,127,208,239,28,94,126,187,232,91,239,26,141,246,72,116,156,245,14,79,58,251,208,214,20,109,167,251,157,65,239,242,228,224,224,188,55,176,154,42,108,239,247,65,239,120,159,225,104,42,237,131,254,160,119,104,41,13,167,128,49,131,143,54,165,68,250,151,93,108,121,167,3,117,25,212,187,20,106,239,98,48,56,57,182,222,235,112,188,181,217,120,159,66,30,118,246,128,137,15,58,32,107,108,54,62,164,112,231,63,250,131,238,247,222,153,245,81,7,149,237,205,198,71,5,250,176,191,15,109,159,50,176,172,181,217,248,148,66,30,95,28,237,65,219,223,116,72,222,218,108,252,77,17,23,200,17,20,114,122,49,176,62,235,208,74,79,179,241,57,29,241,237,172,115,250,221,250,194,27,58,251,251,172,229,242,244,164,127,140,160,188,163,123,216,235,156,113,224,102,227,139,66,177,179,103,53,51,10,97,109,138,66,206,123,135,189,46,96,203,232,68,52,55,21,173,156,156,14,250,40,255,140,90,68,115,83,81,203,81,31,26,50,90,161,109,77,69,39,71,157,223,173,102,70,31,180,173,169,170,98,208,59,181,154,89,77,208,198,166,162,135,111,157,139,111,61,171,153,209,3,111,109,42,106,232,116,187,104,27,25,21,240,214,230,103,85,48,167,157,179,206,224,4,212,248,37,43,27,217,211,84,197,221,63,234,193,138,202,200,27,27,155,239,148,85,114,208,63,236,237,247,207,79,15,59,255,14,235,73,135,86,251,154,239,20,45,29,156,117,190,29,245,64,235,95,82,198,229,210,219,63,249,113,44,23,222,97,239,96,32,23,216,89,255,219,247,129,92,70,93,64,0,86,41,52,208,189,28,92,156,253,219,197,73,255,188,39,81,117,47,123,71,189,179,206,97,186,144,187,151,167,61,24,117,214,255,7,12,125,39,27,127,244,6,157,243,211,239,157,67,5,253,229,249,197,241,193,225,201,15,141,70,183,115,118,118,50,144,203,174,123,217,57,236,255,71,231,12,108,225,147,108,218,239,156,253,93,46,161,238,229,241,201,49,8,242,227,199,246,173,29,25,208,152,68,129,215,137,99,114,51,244,30,58,81,100,63,88,62,185,51,78,134,255,69,70,73,181,70,161,14,34,251,234,134,248,137,0,27,184,55,36,162,96,116,0,135,186,138,236,112,178,111,39,118,174,103,98,199,157,209,136,120,214,216,246,98,66,155,98,207,117,72,212,5,242,174,63,13,166,49,239,26,79,253,81,226,6,62,48,230,5,81,215,179,227,184,74,127,246,157,218,140,255,176,142,167,55,67,18,201,246,118,124,231,38,163,137,2,103,199,68,149,127,43,34,201,52,242,77,248,231,159,211,192,141,137,217,230,32,92,29,2,128,192,180,108,207,145,221,169,110,4,68,72,18,18,69,238,45,137,36,80,170,43,1,116,71,18,59,14,39,182,151,72,32,169,59,1,19,79,253,177,23,220,41,120,152,42,69,255,200,142,162,32,29,47,212,42,186,109,207,253,111,59,114,125,9,128,74,110,241,223,168,97,1,232,216,209,181,217,118,200,216,158,122,137,104,52,219,243,249,26,106,225,142,12,227,96,116,221,86,126,131,74,124,208,60,113,20,101,253,32,195,115,232,34,9,87,252,212,243,82,77,69,228,159,83,18,39,39,145,11,22,98,99,211,41,137,110,220,56,134,95,213,218,108,190,38,33,99,251,150,124,19,70,2,93,94,48,178,189,243,36,0,235,34,91,49,73,250,9,185,169,154,36,14,167,46,181,165,216,172,255,235,249,201,241,86,156,192,76,175,220,241,67,85,154,88,173,214,86,16,71,64,63,136,20,220,46,88,1,181,50,160,232,80,147,212,104,93,21,210,74,145,183,221,113,85,14,93,167,243,173,205,82,92,148,169,208,142,98,146,66,213,218,30,73,12,151,1,200,214,159,174,243,171,205,132,206,22,196,150,27,179,133,193,32,107,95,217,255,173,159,191,96,62,12,144,254,212,166,102,71,176,16,103,191,85,157,96,52,197,101,88,219,178,29,167,106,254,139,89,219,10,198,99,88,99,191,85,205,55,81,112,7,223,147,228,198,171,154,102,13,87,251,57,104,99,26,247,192,142,34,128,225,104,84,220,89,152,89,225,58,223,26,7,81,207,134,245,69,60,130,157,214,238,108,228,17,59,234,251,176,20,110,109,79,180,3,234,90,123,249,78,81,186,227,8,0,144,125,18,77,137,101,89,89,139,172,205,74,108,148,55,111,141,188,0,116,194,228,33,39,7,82,137,200,77,112,75,216,110,98,210,109,98,243,42,34,196,55,243,160,32,89,13,46,34,78,1,20,19,51,21,155,177,109,28,7,6,231,8,165,250,246,205,231,79,239,63,182,11,70,165,202,210,90,253,42,8,212,29,93,183,184,174,235,32,199,185,162,166,137,237,59,30,249,135,27,187,67,215,115,147,135,46,52,92,193,52,103,32,169,245,172,68,222,190,93,23,102,178,53,113,29,135,248,181,153,180,33,13,173,176,43,180,91,92,29,216,104,221,185,190,19,220,109,137,239,173,73,16,39,190,125,67,168,117,135,65,148,228,32,176,177,45,73,130,248,122,183,240,227,208,141,19,226,195,38,109,222,74,190,71,148,111,179,94,60,159,58,85,37,85,63,162,92,183,76,243,207,63,217,175,207,13,241,235,195,135,247,210,8,168,209,200,157,169,106,222,197,173,237,109,115,67,48,182,97,182,204,13,28,181,97,110,223,129,156,219,115,2,248,87,30,204,199,172,1,59,184,252,193,26,245,77,176,54,203,108,138,184,127,137,21,33,100,204,116,36,172,51,34,182,243,128,106,71,219,126,175,171,165,254,177,209,104,32,61,1,28,248,65,72,124,75,98,34,183,9,30,128,126,28,120,4,132,127,5,44,51,72,3,225,86,48,228,50,131,79,200,61,204,95,218,15,64,228,22,25,46,199,246,95,98,111,152,183,83,249,208,197,190,138,128,40,96,209,150,248,151,155,18,65,198,86,153,18,157,1,76,41,53,161,191,200,92,152,183,135,235,155,110,3,153,185,36,209,3,61,150,157,204,41,10,157,91,14,61,68,231,107,176,0,129,97,58,58,157,60,21,12,111,148,242,138,137,15,167,224,212,13,174,97,165,55,106,252,156,109,51,207,134,88,114,79,26,6,206,3,115,118,9,206,27,54,22,225,49,34,205,173,228,33,36,220,103,212,239,244,173,130,99,21,155,18,123,24,251,246,109,81,51,74,8,136,168,93,176,1,80,50,89,135,23,28,138,172,11,92,8,7,243,161,237,92,248,113,129,78,113,110,224,223,221,51,145,207,16,188,149,113,156,132,162,235,243,182,162,158,170,28,69,45,64,176,154,4,137,237,9,122,187,85,157,188,71,252,171,100,178,217,172,201,141,88,87,68,41,56,76,100,8,91,224,117,91,136,58,141,124,180,254,111,103,200,228,142,11,9,206,54,114,191,241,164,25,175,50,62,47,3,22,41,106,101,15,213,136,120,129,237,192,138,210,161,105,76,168,37,13,59,113,19,143,48,171,241,236,33,241,168,25,222,216,174,255,29,78,26,184,89,112,43,76,1,178,248,104,36,167,37,190,88,88,69,126,166,113,19,217,196,194,22,242,147,222,255,149,78,126,111,111,9,41,211,163,223,35,53,56,138,6,193,247,193,209,97,149,45,114,157,9,22,119,202,13,154,101,71,225,228,134,137,111,110,80,48,240,240,169,231,148,4,211,209,132,202,190,149,238,53,181,25,217,10,35,130,106,223,103,55,31,234,119,37,19,55,86,15,70,0,129,173,12,143,188,225,52,73,240,68,1,47,172,202,209,215,241,212,3,133,213,41,5,208,247,170,248,53,103,115,49,9,238,242,204,169,199,151,177,12,17,58,91,46,24,182,153,129,175,37,208,50,112,219,163,252,103,208,98,52,80,170,12,127,175,36,247,112,252,100,177,135,182,195,38,125,113,90,127,170,104,139,112,168,178,163,44,122,207,103,17,163,59,207,103,82,195,146,99,51,122,62,155,52,238,244,124,62,117,52,57,70,135,207,103,20,67,103,207,231,83,195,146,99,115,244,124,54,89,228,238,249,140,102,240,44,92,221,52,216,189,124,241,69,120,53,58,167,46,65,117,61,231,66,228,208,14,58,123,121,156,186,207,98,135,33,61,188,118,60,119,119,199,54,2,255,6,48,145,105,104,85,0,134,205,67,74,116,195,172,85,140,73,68,198,86,5,49,40,237,149,93,254,65,183,153,13,115,103,219,222,221,217,6,148,133,206,144,164,233,184,183,134,235,80,82,26,178,157,109,232,193,177,56,206,2,4,91,58,2,248,26,18,167,203,190,171,51,47,8,194,22,170,105,94,163,126,99,213,180,67,151,81,182,1,122,236,122,224,231,169,183,48,30,130,145,71,64,146,192,205,20,39,102,214,224,46,6,23,34,156,247,102,18,108,178,189,212,4,95,1,141,201,164,237,102,93,53,0,116,69,128,29,43,140,130,155,16,110,78,3,123,104,36,129,193,6,226,47,184,47,98,120,210,8,34,152,106,237,43,115,1,215,113,62,91,12,8,70,84,225,19,220,9,219,35,17,162,152,216,9,226,52,156,128,196,134,31,36,6,185,135,251,179,209,250,227,15,122,7,45,176,185,2,147,98,15,5,82,253,224,89,195,136,46,115,66,24,219,172,9,197,251,38,6,119,105,148,112,29,232,160,210,73,162,170,5,233,108,190,55,107,156,86,174,235,29,116,177,225,169,146,131,144,198,23,80,207,236,167,185,177,38,116,189,102,86,12,54,184,34,90,153,5,97,135,185,97,178,144,197,174,232,163,94,11,244,237,108,51,76,104,34,120,141,127,73,154,172,133,9,132,32,244,66,234,52,34,64,123,39,118,124,114,231,159,70,112,255,142,146,135,106,37,38,96,175,206,41,101,172,82,83,37,190,151,23,185,10,92,91,36,214,189,5,115,220,123,53,193,190,16,213,151,18,45,172,218,168,80,178,221,188,100,21,216,133,130,237,46,152,98,247,213,4,251,66,84,31,43,88,125,167,56,234,151,110,19,208,252,91,181,242,38,246,42,69,91,3,191,202,224,145,82,14,195,54,215,27,215,55,117,71,20,165,96,112,252,176,69,46,39,80,10,84,70,33,55,207,206,239,175,63,79,251,126,193,60,49,184,182,156,66,57,148,164,193,192,95,91,162,185,185,228,220,150,65,239,244,213,69,26,39,36,124,229,153,22,144,200,78,149,190,248,47,119,208,228,163,209,79,190,116,127,89,185,215,40,225,159,182,129,19,112,224,104,199,249,237,85,53,63,182,110,210,182,212,165,213,215,110,38,73,161,69,125,145,152,134,224,0,13,169,214,240,101,11,227,118,213,34,198,182,194,105,60,169,206,238,91,73,92,127,104,165,147,7,95,34,243,50,247,20,70,21,54,149,148,137,86,129,124,126,254,122,97,122,34,239,0,181,37,158,154,107,172,95,190,60,211,24,246,114,119,123,225,91,102,110,35,85,18,10,114,150,178,150,67,126,224,122,100,223,141,67,207,126,184,8,49,190,131,13,85,17,247,84,49,43,121,54,52,8,169,94,109,211,144,14,183,220,178,67,147,7,196,206,147,7,143,224,169,153,69,36,86,2,116,243,165,160,142,40,102,73,6,37,22,196,28,224,152,109,152,95,27,173,230,227,88,139,159,205,27,187,82,149,224,162,172,105,75,126,141,93,167,46,49,90,83,205,141,98,219,70,53,253,161,12,173,155,205,70,195,84,30,172,254,247,38,201,99,116,136,11,182,56,5,25,70,243,159,106,21,58,166,167,176,165,196,10,233,237,15,14,180,23,226,45,131,106,57,115,101,200,93,63,156,38,131,135,112,49,102,124,12,224,136,229,128,18,115,99,177,208,156,255,249,220,9,231,144,61,69,31,252,46,248,136,75,31,130,50,143,145,130,138,75,148,220,110,95,225,214,147,146,220,83,104,238,233,68,121,35,63,220,119,27,212,207,200,251,244,2,140,5,95,171,204,231,158,137,214,172,62,68,59,125,251,84,195,227,224,5,208,129,64,131,39,25,41,195,118,55,223,189,244,173,108,177,219,254,10,23,162,84,230,93,69,230,221,34,153,119,87,147,121,183,80,230,221,18,153,119,159,46,243,151,186,176,173,32,115,246,189,218,252,11,167,95,50,251,167,79,254,101,162,43,143,184,37,170,57,193,173,252,211,75,118,126,121,136,116,170,79,216,4,117,84,79,217,1,211,103,142,52,11,186,149,7,99,79,88,72,242,202,158,94,145,23,218,195,179,184,158,50,1,230,209,22,28,181,253,163,30,221,216,163,91,213,241,79,130,254,249,201,57,125,13,173,102,159,200,19,184,17,180,204,141,232,150,230,196,20,122,208,185,4,217,214,34,111,53,227,5,243,236,217,22,230,8,137,44,130,67,226,243,151,73,186,136,218,162,143,78,251,100,60,142,73,194,250,3,250,155,246,31,131,201,136,241,28,36,51,100,67,65,79,135,12,240,33,215,74,223,116,105,99,135,102,104,58,86,85,129,222,200,96,170,181,85,110,15,92,31,208,48,100,150,197,199,211,184,109,161,206,249,211,46,168,123,45,147,162,96,170,242,56,194,139,131,127,37,146,108,141,8,142,191,200,169,27,241,181,27,134,74,135,41,36,58,95,67,166,120,171,165,190,34,183,207,147,32,44,204,208,208,30,155,197,14,222,128,89,100,39,76,89,205,229,111,252,76,71,254,162,20,219,203,160,184,198,172,188,182,128,73,59,74,138,185,84,25,148,138,59,99,215,45,43,243,148,63,171,184,78,165,149,142,168,87,24,201,74,43,79,115,158,207,7,25,115,0,150,23,2,54,15,91,159,74,14,87,65,42,113,144,150,57,133,219,231,216,245,137,99,130,220,208,5,12,198,198,50,49,228,85,47,212,61,118,163,56,49,4,23,198,56,144,105,214,200,137,42,136,87,151,87,227,37,196,147,49,164,117,203,90,209,68,242,34,250,17,5,32,32,31,180,40,229,179,101,244,238,67,26,198,84,164,179,12,243,134,105,124,11,18,28,144,91,214,175,46,211,165,44,178,29,111,213,169,60,91,67,75,233,136,223,204,21,176,180,207,231,172,246,52,81,87,219,70,65,235,43,164,235,172,202,245,130,124,158,165,219,25,205,83,103,14,214,218,236,255,211,222,36,252,1,145,182,159,61,99,252,107,63,184,243,13,220,169,240,153,145,190,13,178,4,71,5,44,77,110,83,23,179,108,221,181,68,66,209,219,183,178,113,71,43,84,171,205,194,40,24,145,56,238,249,246,208,35,142,140,85,101,240,168,99,52,100,90,58,29,187,166,136,156,46,116,147,92,103,133,11,102,104,251,196,211,188,43,117,88,129,107,149,14,88,20,14,224,97,58,196,201,65,126,202,54,216,219,115,116,226,73,112,199,125,252,124,231,196,117,72,53,43,24,203,210,2,83,181,25,159,186,158,32,196,130,80,155,178,52,133,23,198,108,242,82,20,241,153,214,157,136,150,180,200,68,180,200,146,18,209,192,106,72,196,151,44,25,193,89,16,145,173,173,179,129,27,176,172,187,225,142,5,124,215,68,246,116,225,20,82,222,5,211,10,183,42,155,146,63,206,216,34,142,74,249,88,43,177,73,76,26,160,217,18,155,60,174,119,73,191,204,218,22,193,92,66,37,25,128,103,1,252,108,252,226,247,57,61,45,128,69,10,211,124,46,60,173,170,60,238,90,8,201,94,227,149,52,222,27,96,15,78,37,75,217,183,128,65,59,126,240,71,134,76,191,95,224,95,175,209,156,124,188,64,89,246,157,237,38,134,3,171,93,3,17,151,18,132,195,74,150,65,112,17,66,59,233,59,150,57,78,45,147,94,203,204,13,29,130,93,109,16,123,173,164,127,68,181,10,0,29,207,189,242,205,186,233,145,113,98,46,132,190,155,184,9,72,62,180,71,176,12,77,216,157,34,59,92,60,2,236,39,66,115,0,240,120,4,123,169,183,26,248,230,195,99,7,220,47,31,192,186,7,65,88,45,23,152,124,95,202,171,82,83,207,24,254,193,138,9,174,197,136,196,33,108,203,132,107,114,76,48,239,88,194,224,174,39,32,48,131,52,153,198,235,88,74,92,155,37,147,40,184,51,240,130,199,178,213,77,68,110,156,17,219,49,206,73,132,43,139,182,183,12,184,233,103,48,108,152,21,122,134,8,35,186,164,233,214,140,190,4,165,215,115,145,51,157,130,169,229,65,11,78,76,16,10,158,168,101,87,18,44,202,43,236,249,217,135,99,185,176,82,226,146,162,212,157,241,245,197,206,56,12,249,197,198,224,65,95,230,150,114,168,71,157,235,121,12,91,43,123,130,20,250,153,174,221,124,62,175,55,27,141,70,189,239,212,116,141,44,144,120,185,244,74,85,193,198,44,234,214,203,5,202,33,145,101,165,180,136,238,192,172,222,135,167,67,177,211,31,144,88,226,29,132,183,179,197,149,17,84,236,209,13,14,100,4,255,211,168,5,135,110,63,122,155,215,223,127,178,123,63,207,164,146,156,51,50,11,56,167,111,37,139,88,247,203,56,87,168,224,114,91,64,131,189,76,44,34,146,172,66,68,164,243,21,146,192,4,188,133,20,236,225,10,52,88,160,121,145,162,121,36,122,161,178,87,32,164,102,79,179,238,186,27,227,190,203,243,14,120,200,80,144,1,31,33,172,154,142,27,83,31,1,142,232,153,44,14,1,112,62,82,231,98,136,109,10,113,154,114,144,1,153,134,197,220,201,12,80,92,112,245,98,254,208,97,100,61,112,121,229,229,35,118,204,93,168,66,78,121,145,138,82,159,194,18,76,91,101,83,8,71,203,231,16,142,180,73,104,49,192,114,196,227,21,16,143,203,16,99,254,110,57,234,21,228,30,14,203,80,99,170,117,57,106,111,5,212,94,25,106,154,29,93,142,59,90,1,119,84,128,91,219,38,197,35,58,183,24,60,190,9,53,152,152,85,9,178,194,99,12,67,102,55,205,212,116,0,193,232,154,89,78,102,89,217,64,229,150,232,123,167,134,68,41,17,20,72,244,42,73,129,9,174,245,203,112,233,37,182,41,58,153,186,195,167,212,76,31,194,23,177,145,133,97,11,90,244,139,20,237,12,238,70,30,119,9,91,75,208,139,204,109,86,74,166,228,96,167,245,108,110,12,206,251,8,214,42,79,9,166,64,86,233,233,164,252,205,131,209,208,82,78,41,237,168,212,143,44,216,49,112,2,112,67,24,145,234,246,127,254,177,191,177,125,85,55,233,125,131,229,133,211,124,119,250,76,221,42,56,247,212,123,11,182,240,3,111,62,231,89,229,165,7,102,201,192,54,221,200,84,6,71,182,231,13,237,209,245,57,129,219,193,186,101,162,86,76,102,175,170,120,196,160,148,91,33,136,185,176,55,21,132,201,66,131,89,43,167,91,231,84,219,243,185,168,63,148,121,55,169,156,233,181,106,150,70,6,172,165,161,134,175,166,65,131,10,248,146,150,9,43,108,224,99,90,203,52,219,180,233,14,216,44,70,135,61,136,8,255,167,240,165,239,9,218,91,124,165,246,231,159,242,136,46,207,115,195,103,73,229,125,127,21,188,95,23,161,77,171,29,89,17,39,104,127,113,165,164,94,60,198,95,10,115,213,75,165,117,71,153,74,136,167,23,161,209,52,179,226,146,52,246,134,86,148,173,197,166,39,235,17,148,104,14,213,238,134,170,109,99,132,219,135,85,73,238,2,252,179,36,211,27,63,22,16,168,90,4,176,35,199,72,88,121,41,102,231,22,7,47,104,161,195,228,163,168,156,96,111,176,230,206,54,52,237,76,162,109,124,156,229,97,144,52,105,140,190,209,210,218,136,204,163,91,90,109,247,148,169,196,236,111,21,80,148,98,78,88,210,1,212,22,48,88,204,9,125,144,20,17,70,253,181,26,89,19,75,60,147,139,34,217,204,228,161,232,8,114,242,144,155,178,210,147,93,230,216,13,146,224,66,80,161,23,172,125,253,25,55,183,250,213,110,185,254,41,33,145,37,100,45,203,58,2,156,52,44,40,80,202,46,137,111,217,90,227,127,189,101,39,6,149,82,93,195,9,38,180,172,49,184,38,21,77,117,104,208,127,55,49,38,147,175,219,65,100,187,102,97,74,163,160,135,247,17,80,139,77,105,98,120,137,17,133,205,2,40,124,120,36,117,32,41,240,41,100,249,246,33,40,50,119,159,210,163,126,125,217,44,121,245,18,250,98,0,169,220,17,216,144,186,65,29,5,76,134,48,213,74,167,34,72,118,230,87,118,245,196,9,224,150,1,43,188,202,189,77,112,203,166,137,204,198,203,53,194,11,133,68,217,176,200,40,105,154,95,165,255,1,182,0,11,191,36,110,13,255,185,112,0,82,123,194,143,77,225,69,26,124,156,201,23,50,110,8,156,166,235,243,54,106,115,220,12,41,181,97,112,95,97,156,11,198,3,191,139,114,17,140,226,159,240,96,18,162,206,40,45,62,88,194,185,41,118,138,157,109,42,25,69,116,185,83,64,136,16,46,68,130,89,241,184,190,187,51,245,24,215,90,165,26,211,181,90,17,106,232,202,70,14,77,69,217,133,160,92,219,116,234,88,220,202,202,208,222,190,249,242,233,99,163,157,22,179,45,163,206,74,40,87,101,32,3,173,241,16,61,153,7,90,110,186,42,11,58,176,198,129,247,100,14,104,129,230,170,28,232,192,26,7,195,133,28,108,51,115,208,30,88,152,69,125,53,129,181,212,122,198,110,116,83,41,97,149,151,104,174,200,43,135,46,230,118,36,184,61,249,59,50,42,86,223,206,54,216,178,186,91,48,255,70,24,186,178,42,213,139,66,186,172,86,88,240,124,132,190,220,217,210,214,55,33,182,208,41,157,138,113,227,130,32,26,240,191,125,111,85,154,141,134,146,42,166,110,120,84,26,250,113,87,196,48,191,217,192,42,165,167,81,33,4,197,88,176,163,210,35,71,56,19,25,199,79,200,137,77,135,31,194,212,127,106,13,225,34,116,221,54,74,78,100,156,57,13,206,241,60,56,54,119,118,193,43,157,42,28,2,244,182,33,0,249,61,140,201,15,20,189,173,48,168,184,162,58,147,0,157,30,229,143,228,152,133,250,178,169,123,101,76,42,113,195,66,22,185,107,44,253,4,26,124,123,172,16,69,200,142,51,149,82,215,130,125,133,244,153,47,46,200,143,221,171,105,68,40,78,94,188,193,139,125,161,99,100,243,188,195,140,171,169,116,209,15,64,160,226,167,222,61,199,255,163,127,120,104,236,245,140,142,65,155,141,114,147,97,243,226,73,121,175,101,30,236,174,193,121,163,31,198,54,254,105,48,88,147,55,33,19,51,113,140,49,28,162,15,235,234,46,96,99,121,10,137,130,27,124,51,101,156,210,38,201,233,174,10,61,132,171,175,148,35,59,100,67,144,113,17,174,96,154,128,60,82,224,144,138,178,236,47,1,234,175,170,89,151,186,220,51,205,92,244,50,101,34,226,61,156,48,180,5,69,16,106,128,166,34,194,160,21,30,161,81,235,49,244,136,174,12,209,240,240,65,182,180,66,132,148,30,131,146,7,149,138,43,245,87,169,52,201,34,92,215,230,158,185,54,105,23,221,37,197,29,143,66,156,187,50,175,82,162,241,40,10,218,13,124,121,53,196,163,112,107,113,132,37,73,200,143,66,172,122,158,221,108,118,112,97,182,112,209,149,168,44,91,182,208,208,181,171,56,143,249,151,217,123,106,200,69,163,10,237,89,88,234,124,254,63,184,71,124,138,146,90,0,0 };
