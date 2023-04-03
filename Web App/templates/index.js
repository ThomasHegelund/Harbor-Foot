const uncharged_boats = document.querySelectorAll('#uncharged-boats-list li');
const uncharged_boats_list = document.querySelector('#uncharged-boats-list');
const charged_boats_list = document.querySelector('#charged-boats-list');
console.log(charged_boats_list)

// console.log(uncharged_boats_list)
uncharged_boats.forEach(boat => {
    input = boat.querySelector('input');
    input.addEventListener('change', function(event){
        event.preventDefault();
        moved_boat = event.target.parentNode.parentNode;
        // moved_boat = move_boat.cloneNode(true);
        // move_boat.del
        charged_boats_list.append(moved_boat)
    });
});


