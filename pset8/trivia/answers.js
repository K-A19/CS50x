window.onload = function() {
    var red = document.querySelector('#red');
    red.addEventListener('click', function() {
        red.style.backgroundColor = 'red';
        document.querySelector('#status').textContent = "Incorrect";
        document.querySelector('#status').style.visibility = 'visible';
        });

    var green = document.querySelector('#green');
    green.addEventListener('click', function() {
        green.style.backgroundColor = 'red';
        document.querySelector('#status').textContent = "Incorrect";
        document.querySelector('#status').style.visibility = 'visible';
        });

    var yellow = document.querySelector('#yellow');
    yellow.addEventListener('click', function() {
        yellow.style.backgroundColor = 'red';
        document.querySelector('#status').textContent = "Incorrect";
        document.querySelector('#status').style.visibility = 'visible';
        });

    var blue = document.querySelector('#blue');
    blue.addEventListener('click', function() {
        blue.style.backgroundColor = 'green';
        document.querySelector('#status').textContent = "Correct!";
        document.querySelector('#status').style.visibility = 'visible';
        });

    var form = document.getElementById('f2');
    form.addEventListener('submit', function(e) {
        var answer = document.getElementById('answer').value;
        if (answer == "Joe Biden")
        {
            document.querySelector('#status2').textContent = "Correct!";
            document.querySelector('#status2').style.visibility = 'visible';
        }
        else
        {
            document.querySelector('#status2').textContent = "Incorrect";
            document.querySelector('#status2').style.visibility = 'visible';
        }

        e.preventDefault()
    });
}