(function ($) {
  Drupal.behaviors.counterfield = function(context) {
    for (var i in Drupal.settings.counterfields) {
      var setting = Drupal.settings.counterfields[i];
      var flashvars = {
        pathToJSONData: setting.pathToJSON, 
      };
      var params = { wmode: "transparent"};
    //swfobject.embedSWF(setting.pathToSWF, setting.uniqueDIV, setting.width, setting.height, '9.0.0', '', flashvars, params);
    var item = $('#' + setting.uniqueDIV + ' h4');
    counterfield_js_count(item, setting.start_number, setting.rate);
    }
  }
})(jQuery);  //compatibility wrapper

function counterfield_js_count(item, currentValue, rate) {
	setInterval(
	    function() {
        currentValue += rate / 1440;
        item.html(counterfieldFormatAsDollars(currentValue));
	    },
	    41
	);
}

function counterfieldFormatAsDollars (amount) {
  amount = Math.round(amount*100)/100;
  var amount_str = String(amount);   
  var amount_array = amount_str.split("."); 
  if (amount_array[1] == undefined)
  {
    amount_array[1] = "00";
  }   
  if (amount_array[1].length == 1) 
  {
    amount_array[1] += "0";
  }
  var dollar_array = new Array();
  var start;
  var end = amount_array[0].length;
  while (end > 0)
  {
    start = Math.max(end - 3, 0);   
    dollar_array.unshift(amount_array[0].slice(start, end));
    end = start;  
  }
  amount_array[0] = dollar_array.join(",");
  return ("$" + amount_array.join("."));
}
