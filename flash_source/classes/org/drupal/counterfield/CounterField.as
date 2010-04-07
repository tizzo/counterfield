﻿package org.drupal.counterfield{	//Import Flash Classes	import flash.display.MovieClip;	import flash.display.DisplayObject;	import flash.events.Event;	import flash.events.MouseEvent;	import flash.events.IOErrorEvent;	import flash.text.TextField;	import flash.net.URLRequest;	import flash.net.URLLoader;	import flash.net.navigateToURL;	import flash.display.LoaderInfo;	import com.adobe.serialization.json.JSON;	import gs.TweenLite;	import gs.easing.*;	import flash.system.System;		public class CounterField extends MovieClip	{		// Define variables		var currentValue:Number = new Number();		// Hourly rate to increase the value		var rate:Number = new Number();		var pathToJSONData:String = new String();		var embedCodeY:Number = new Number();		var learnMoreDestination:String = new String();		var embedCode:String = new String();				// Constructor function.		public function CounterField() {			// Load configration from embed code			var paramObj:Object = LoaderInfo(this.root.loaderInfo).parameters;			if (!paramObj.pathToJSONData) {				pathToJSONData = 'http://sftest.local/counterfield/json/node/4/field_my_counter/0';			}			else {				trace ('failz');				pathToJSONData = paramObj.pathToJSONData;			}			stop();			// Get the necessary data.			getData();			KeystoneTemplate_mc.showEmbedCode_mc.addEventListener(MouseEvent.CLICK, showEmbedCode);			KeystoneTemplate_mc.embedCode_mc.closeButton_mc.addEventListener(MouseEvent.CLICK, hideEmbedCode);			KeystoneTemplate_mc.embedCode_mc.embedCodeCopy_mc.addEventListener(MouseEvent.CLICK, copyEmbedCode);			KeystoneTemplate_mc.learnMore_mc.addEventListener(MouseEvent.CLICK, goToLearnMoreLink);			embedCodeY = KeystoneTemplate_mc.embedCode_mc.y;			KeystoneTemplate_mc.embedCode_mc.y = 0 - KeystoneTemplate_mc.embedCode_mc.height;		}				private function getData()		{			var request = new URLRequest();			var loader = new URLLoader();			request.url = pathToJSONData;			loader.load(request);			loader.addEventListener(Event.COMPLETE, decodeJSON);		}		function decodeJSON(event:Event):void		{			var loader:URLLoader = URLLoader(event.target);			var data:Object = JSON.decode(loader.data);			currentValue = data.currentValue;			rate = data.rate;			learnMoreDestination = data.moreInfoURL;			var swfPath:String = new String(data.swfPath);			embedCode = '<object type="application/x-shockwave-flash" data="' + swfPath + '" width="680" height="220" style="visibility: visible; "><param name="wmode" value="transparent"><param name="flashvars" value="pathToJSONData=' + pathToJSONData + '"></object>';			KeystoneTemplate_mc.embedCode_mc.EmbedCode_txt.text = embedCode;									// If we get here we should have all of our data,			// start updating the COUNTING!			addEventListener(Event.ENTER_FRAME, updateCounter);		}		// Update the counter with new functions		public function updateCounter(e:Event) {			var updatedValue:Number = currentValue + ((rate / 3600) / 24);			currentValue = updatedValue;			KeystoneTemplate_mc.taxCounter_txt.text = formatAsDollars(updatedValue);		}				public function formatAsDate (date:Number):String {			return '';		}				public function showEmbedCode (e:Event) {			var offset:Number = embedCodeY;			TweenLite.to(KeystoneTemplate_mc.embedCode_mc, 1, {y:offset, ease:Bounce.easeOut});		}				public function hideEmbedCode (e:Event) {			var offset:Number = 0 - KeystoneTemplate_mc.embedCode_mc.height;			TweenLite.to(KeystoneTemplate_mc.embedCode_mc, 0.5, {scaleX:0, ease:Bounce.easeOut});			TweenLite.to(KeystoneTemplate_mc.embedCode_mc, 0.2, {alpha:0, ease:Strong.easeOut});			TweenLite.to(KeystoneTemplate_mc.embedCode_mc, 0.5, {scaleY:0, ease:Strong.easeIn, onComplete:cleanUp});		}				public function cleanUp () {			KeystoneTemplate_mc.embedCode_mc.scaleX = 1;			KeystoneTemplate_mc.embedCode_mc.scaleY = 1;			KeystoneTemplate_mc.embedCode_mc.y = 0 - KeystoneTemplate_mc.embedCode_mc.height;			KeystoneTemplate_mc.embedCode_mc.alpha = 1;		}				public function copyEmbedCode (e:Event) { 			System.setClipboard(embedCode);		}				public function goToLearnMoreLink (e:Event) {			navigateToURL(new URLRequest(learnMoreDestination));		}				// This function shamelessly stolen from Adobe's website. :-D		public function formatAsDollars (amount:Number):String {			// return a 0 dollar value if amount is not valid  			// (you may optionally want to return an empty string)  			if (isNaN(amount))			{				return "$0.00";			}   			// round the amount to the nearest 100th  			amount = Math.round(amount*100)/100;			// convert the number to a string  			var amount_str:String = String(amount);   			// split the string by the decimal point, separating the  			// whole dollar value from the cents. Dollars are in  			// amount_array[0], cents in amount_array[1]			var amount_array = amount_str.split("."); 			// if there are no cents, add them using "00"			if (amount_array[1] == undefined)			{				amount_array[1] = "00";			}   			// if the cents are too short, add necessary "0" 			if (amount_array[1].length == 1) 			{				amount_array[1] += "0";			}			// add the dollars portion of the amount to an			// array in sections of 3 to separate with commas			var dollar_array:Array = new Array();			var start:Number;			var end:Number = amount_array[0].length;			while (end > 0)			{				start = Math.max(end - 3, 0);   				dollar_array.unshift(amount_array[0].slice(start, end));				end = start;  			}			// assign dollar value back in amount_array with			// the a comma delimited value from dollar_array			amount_array[0] = dollar_array.join(",");			// finally construct the return string joining			// dollars with cents in amount_array			return ("$" + amount_array.join("."));		}	} }