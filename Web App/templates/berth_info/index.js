const return_date_input = document.querySelector('#berth-info input[type="date"]');

return_date_input.addEventListener('change', function(event){
    event.preventDefault();
    new_return_date = return_date_input.value;
    berth_uuid = window.location.href.split('/').at(-1);

    post_update_berth_return_date(berth_uuid, new_return_date);
})


function post_update_berth_return_date(berth_uuid, return_date)
{
    body = create_post_request_json_body(berth_uuid, return_date)
    create_update_berth_return_date_post_request(body);
}


function create_post_request_json_body(berth_uuid, return_date)
{
    return JSON.stringify(
        { 
            "berth_uuid": berth_uuid,
            "return_date": return_date
        }
    )
}

function create_update_berth_return_date_post_request(body)
{
    fetch('/update_berth_return_date', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: body
    })
}


