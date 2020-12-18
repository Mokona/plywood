var highlighted = null;
var selected = null;

function highlight(elementID) {
    if (highlighted) {
        highlighted.classList.remove("highlighted");
    }
    highlighted = document.getElementById(elementID);
    if (highlighted) {
        highlighted = highlighted.parentElement;
        if (highlighted) {
            highlighted.classList.add("highlighted");
        }
    }
}

function savePageState() {
    stateData = {
        path: location.pathname + location.hash,
        pageYOffset: window.pageYOffset
    };
    window.history.replaceState(stateData, null);
}

function expandToItem(targetItem) {
    var parent = targetItem.parentElement;
    if (parent.tagName == "A") {
        parent = parent.parentElement;
    }
    while (parent.tagName == "UL") {
        parent.classList.add("active");
        var li = parent.previousElementSibling;
        if (li) {
            if (li.tagName == "A") {
                li = li.children[0];
            }
            li.classList.add("caret-down");
        }                        
        parent = parent.parentElement;
    }
}

activeScrollers = [];

function updateScrollers(timestamp) {    
    for (var i = 0; i < activeScrollers.length; i++) {
        props = activeScrollers[i];
        if (props.start === null) {
            props.start = timestamp;
        } else {
            var f = (timestamp - props.start) / 100;
            if (f < 1) {
                props.element.scrollTop = props.scrollTo * f + props.scrollFrom * (1 - f);
            } else {
                props.element.scrollTop = props.scrollTo;
                activeScrollers.splice(i, 1);
                i--;
            }
        }
    }
    if (activeScrollers.length > 0) {
        requestAnimationFrame(updateScrollers);
    }
}

function smoothScrollIntoView(name, container, item, scrollToTop) {
    if (!item)
        return;
    var amount;
    if (scrollToTop) {        
        amount = item.getBoundingClientRect().top - 50;  // hardcoded header size        
    } else {
        amount = item.getBoundingClientRect().top - container.getBoundingClientRect().top;
        if (amount >= 0) {
            var containerBottom = Math.min(document.documentElement.clientHeight, container.getBoundingClientRect().bottom);
            amount = item.getBoundingClientRect().bottom - containerBottom;
            if (amount <= 0)
                return; // No need to scroll
        }
    }

    if (amount != 0) {       
        if (activeScrollers.length == 0) {
            requestAnimationFrame(updateScrollers);
        } 
        // Find existing item
        var props = null;
        for (var p in activeScrollers) {
            if (p.element === container) {
                props = p;
                break;
            }
        }
        if (!props) {
            props = {};
            activeScrollers.push(props)
        }
        props.element = container;
        props.start = null;
        var t = container.scrollTop;
        var maxScrollTop = container.scrollHeight - container.clientHeight;
        props.scrollTo = Math.min(t + amount, maxScrollTop);
        props.scrollFrom = Math.min(Math.max(t, props.scrollTo - 500), props.scrollTo + 500);
    }
}

function showTOCEntry(pathToMatch) {
    // Select appropriate TOC entry
    if (selected) {
        selected.classList.remove("selected");
        selected = null;
    }
    var sidebar = document.querySelector(".sidebar");
    var list = sidebar.getElementsByTagName("li");
    for (var j = 0; j < list.length; j++) {
        var li = list[j];
        if (li.parentElement.getAttribute("href") == pathToMatch) {
            selected = li;
            li.classList.add("selected");
            expandToItem(li);
            smoothScrollIntoView("sidebar", sidebar, li, false);
        }
    }
}

var currentRequest = null;
var currentLoadingTimer = null;
var spinnerShown = false;

function navigateTo(dstPath, forward, pageYOffset) {
    if (currentRequest !== null) {
        currentRequest.abort();
        currentRequest = null;
    }

    // Update history
    if (forward) {
        history.pushState(null, null, dstPath);
    }

    // Extract anchor from dstPath
    var anchorPos = dstPath.indexOf("#");
    var anchor = (anchorPos >= 0) ? dstPath.substr(anchorPos + 1) : "";

    // Show appropriate TOC entry
    var pathToMatch = (anchorPos >= 0) ? dstPath.substr(0, anchorPos) : dstPath;
    showTOCEntry(pathToMatch);

    // Issue AJAX request for new page
    currentRequest = new XMLHttpRequest();
    currentRequest.onreadystatechange = function() {
        if (currentRequest !== this)
            return;
        if (this.readyState == 4 && this.status == 200) {
            currentRequest = null; // Completed

            // Extract page title
            var n = this.responseText.indexOf("\n");
            document.title = this.responseText.substr(0, n);

            // Apply article
            spinnerShown = false;
            var article = document.getElementById("article");
            article.innerHTML = this.responseText.substr(n + 1);
            replaceLinks(article);

            // Scroll to desired position
            if (anchor != "") {
                highlight(anchor);
            }
            if (forward && anchor != "") {
                smoothScrollIntoView("document", document.documentElement, document.getElementById(anchor).parentElement, true);
            } else {
                window.scrollTo(0, pageYOffset);
            }
            if (location.pathname != dstPath) {
                // Should never happen
                console.log("location.pathname out of sync: '" + location.pathname + "' != '" + dstPath + "'");
            }
            savePageState();
        }
    };
    currentRequest.open("GET", "/content?path=" + dstPath, true);
    currentRequest.send();            

    // Set timer to show loading spinner
    if (currentLoadingTimer !== null) {
        window.clearTimeout(currentLoadingTimer);
    }
    var showSpinnerForRequest = currentRequest;
    currentLoadingTimer = window.setTimeout(function() {
        if (spinnerShown || currentRequest !== showSpinnerForRequest)
            return;
        spinnerShown = true;
        var article = document.getElementById("article");
        article.innerHTML = 
'<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="32px" height="32px" viewBox="0 0 100 100" style="margin: 0 auto;">\
<g>\
  <circle cx="50" cy="50" fill="none" stroke="#dbe6e8" stroke-width="12" r="36" />\
  <circle cx="50" cy="50" fill="none" stroke="#4aa5e0" stroke-width="12" r="36" stroke-dasharray="50 180" />\
  <animateTransform attributeName="transform" type="rotate" repeatCount="indefinite" dur="1s" values="0 50 50;360 50 50" \
  keyTimes="0;1" />\
</g>\
</svg>'
    }, 750);
}

function replaceLinks(root) {
    var list = root.getElementsByTagName("a");
    for (var i = 0; i < list.length; i++) {
        var a = list[i];
        var path = a.getAttribute("href");
        if (path.substr(0, 6) == "/docs/") {
            a.onclick = function() {
                savePageState();
                navigateTo(this.getAttribute("href"), true, 0);
                return false;
            }
        }
    }
}

function onEndTransition(evt) {
    this.removeEventListener('transitionend', onEndTransition);
    this.style.removeProperty("display");
    this.style.removeProperty("transition");
    this.style.removeProperty("height");
}

window.onload = function() { 
    if ('scrollRestoration' in history) {
        history.scrollRestoration = 'manual';
    }

    highlight(location.hash.substr(1));

    var list = document.getElementsByClassName("caret");
    for (var i = 0; i < list.length; i++) {
        list[i].addEventListener("click", function() {
            var childList = this.nextElementSibling || this.parentElement.nextElementSibling;
            if (this.classList.contains("caret-down")) {
                // Collapse
                this.classList.remove("caret-down")
                childList.style.display = "block";
                childList.style.height = childList.scrollHeight + "px";
                childList.classList.remove("active");
                requestAnimationFrame(function() {
                    childList.style.transition = "height 0.15s ease-out";
                    requestAnimationFrame(function() {
                        childList.style.height = "0px";
                        childList.addEventListener('transitionend', onEndTransition);
                    });
                });
            } else {
                // Expand
                this.classList.add("caret-down");
                childList.style.display = "block";
                childList.style.transition = "height 0.15s ease-out";
                childList.style.height = childList.scrollHeight + "px";
                childList.classList.add("active");
                childList.addEventListener('transitionend', onEndTransition);
            }
        });
    }

    var sidebar = document.querySelector(".sidebar");
    replaceLinks(sidebar);
    replaceLinks(document.getElementById("article"));
    selected = sidebar.querySelector(".selected");
}

window.onhashchange = function() { 
    highlight(location.hash.substr(1));
}

window.onscroll = function() {
    savePageState();
}

window.addEventListener("popstate", function(evt) {
    if (evt.state) {
        navigateTo(evt.state.path, false, evt.state.pageYOffset);
    }
});
